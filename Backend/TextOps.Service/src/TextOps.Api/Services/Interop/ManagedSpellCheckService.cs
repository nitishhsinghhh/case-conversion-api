// SPDX-License-Identifier: Apache-2.0

/**************************************************************************************************
 * File         : ManagedSpellCheckService.cs
 *
 * Copyright    : (c) 2016–2026 Nitish Singh. All rights reserved.
 * License      : Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Description  : Managed service wrapper facilitating platform-agnostic communication with the
 * native unmanaged C++ spellchecking engine. Orchestrates dynamic library loading,
 * thread-safe lookup isolation, memory guards, and cross-boundary telemetry.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author         Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-05-25     Nitish Singh   Initial implementation with dynamic native library 
 *                                           resolution.
 **************************************************************************************************/

using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;

namespace TextOps.Api.Services.Interop
{
    /// <summary>
    /// Represents the immutable structured result from a spellcheck verification pass.
    /// </summary>
    public record SpellCheckResult(bool IsCorrect, string[] Suggestions);

    public interface IManagedSpellCheckService : IDisposable
    {
        SpellCheckResult VerifyWord(string word);
        void AddWordToPersonalDictionary(string word);
    }

    public sealed class ManagedSpellCheckService : IManagedSpellCheckService
    {
        #region Native Function Pointers & Delegates

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr CreateSpellCheckerDelegate();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void FreeSpellCheckerDelegate(IntPtr handle);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private delegate int LoadMainDictionaryDelegate(IntPtr handle, string dictDirectoryPath);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void LoadPersonalDictionaryDelegate(IntPtr handle);

        // Explicitly switched to IntPtr for the word buffer, and a 'ref' pointer for double indirection
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate int CheckWordABIDelegate(IntPtr handle, IntPtr wordPtr, ref IntPtr rawSuggestionsPtr);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private delegate void InsertPersonalWordDelegate(IntPtr handle, string word);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void FreeSuggestionsBufferDelegate(IntPtr bufferPtr);

        #endregion

        #region Private Fields

        private readonly IntPtr _libraryHandle;
        private readonly IntPtr _rawEngineContext;
        private readonly object _lockRoot = new();
        private bool _disposed;

        // Dynamic Function Pointers aligned with stable C++ exports
        private readonly FreeSpellCheckerDelegate _freeSpellChecker;
        private readonly LoadMainDictionaryDelegate _loadMainDictionary;
        private readonly LoadPersonalDictionaryDelegate _loadPersonalDictionary;
        private readonly CheckWordABIDelegate _checkWordABI;
        private readonly InsertPersonalWordDelegate _insertPersonalWord;
        private readonly FreeSuggestionsBufferDelegate _freeSuggestionsBuffer;

        private static readonly ActivitySource _activitySource = new("CaseConversion.Engine");

        #endregion

        #region Constructors / Finalizers

        /// <summary>
        /// Initializes the native wrapper, loading OS-specific modules dynamically at runtime.
        /// </summary>
        /// <param name="mainDictDirectoryPath">The target absolute folder path containing dictionary files.</param>
        public ManagedSpellCheckService(string mainDictDirectoryPath)
        {
            if (string.IsNullOrWhiteSpace(mainDictDirectoryPath))
            {
                throw new ArgumentException("Dictionary storage destination path cannot be null or empty.", nameof(mainDictDirectoryPath));
            }

            // 1. Resolve OS-Specific Binary Extensions

            string dllName = RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? "libLexisCore.dll" :
                             RuntimeInformation.IsOSPlatform(OSPlatform.Linux)   ? "libLexisCore.so" :
                             RuntimeInformation.IsOSPlatform(OSPlatform.OSX)     ? "libLexisCore.dylib" :
                             throw new PlatformNotSupportedException("The executing operating system platform is not supported.");

            string libraryPath = Path.Join(AppContext.BaseDirectory, dllName);

            _libraryHandle = NativeLibrary.Load(libraryPath);

            if (_libraryHandle == IntPtr.Zero)
            {
                throw new DllNotFoundException($"Unable to load native boundary engine library at path resource: {libraryPath}");
            }
 
            // 2. Bind Unmanaged Function Exports to Delegates (Fixed entry-point strings match your exact C++ signatures)
            var createSpellChecker = Marshal.GetDelegateForFunctionPointer<CreateSpellCheckerDelegate>(NativeLibrary.GetExport(_libraryHandle, "createSpellChecker"));
            _freeSpellChecker = Marshal.GetDelegateForFunctionPointer<FreeSpellCheckerDelegate>(NativeLibrary.GetExport(_libraryHandle, "freeSpellChecker"));
            _loadMainDictionary = Marshal.GetDelegateForFunctionPointer<LoadMainDictionaryDelegate>(NativeLibrary.GetExport(_libraryHandle, "loadMainDictionary"));
            _loadPersonalDictionary = Marshal.GetDelegateForFunctionPointer<LoadPersonalDictionaryDelegate>(NativeLibrary.GetExport(_libraryHandle, "loadPersonalDictionary"));
            _checkWordABI = Marshal.GetDelegateForFunctionPointer<CheckWordABIDelegate>(NativeLibrary.GetExport(_libraryHandle, "checkWordABI"));
            _insertPersonalWord = Marshal.GetDelegateForFunctionPointer<InsertPersonalWordDelegate>(NativeLibrary.GetExport(_libraryHandle, "insertPersonalWord"));
            _freeSuggestionsBuffer = Marshal.GetDelegateForFunctionPointer<FreeSuggestionsBufferDelegate>(NativeLibrary.GetExport(_libraryHandle, "freeSuggestionsBuffer"));

            // 3. Allocate the Native Engine Object Context Heap Memory
            _rawEngineContext = createSpellChecker();
            if (_rawEngineContext == IntPtr.Zero)
            {
                throw new ExternalException("Fatal error: Instantiation of unmanaged SpellChecker engine context failed.");
            }

            try
            {
                // Synchronize persistent local Trie dictionary data files
                _loadPersonalDictionary(_rawEngineContext);

                // Mount primary Nuspell dictionary data matrices
                int loadStatus = _loadMainDictionary(_rawEngineContext, mainDictDirectoryPath);
                if (loadStatus == 0)
                {
                    // Fallback lookup hook strategy for macOS/Homebrew deployments if host path misses
                    string localMacFallback = "/opt/homebrew/share/hunspell/en_US";
                    if (Directory.Exists("/opt/homebrew/share/hunspell") && mainDictDirectoryPath != localMacFallback)
                    {
                        loadStatus = _loadMainDictionary(_rawEngineContext, localMacFallback);
                    }
                }

                if (loadStatus == 0)
                {
                    Console.WriteLine("[WARN] Native SpellCheck engine initialized with baseline suggestions disabled (Main Dictionary not found).");
                }
            }
            catch (SEHException ex)
            {
                _freeSpellChecker(_rawEngineContext);
                NativeLibrary.Free(_libraryHandle);
                throw new ExternalException("Fatal pipeline exception during native engine bootstrapping initialization phase.", ex);
            }
            catch (IOException ex)
            {
                _freeSpellChecker(_rawEngineContext);
                NativeLibrary.Free(_libraryHandle);
                throw new ExternalException("Fatal pipeline exception during native engine bootstrapping initialization phase.", ex);
            }
            catch (UnauthorizedAccessException ex)
            {
                _freeSpellChecker(_rawEngineContext);
                NativeLibrary.Free(_libraryHandle);
                throw new ExternalException("Fatal pipeline exception during native engine bootstrapping initialization phase.", ex);
            }
            catch (ExternalException ex)
            {
                _freeSpellChecker(_rawEngineContext);
                NativeLibrary.Free(_libraryHandle);
                throw new ExternalException("Fatal pipeline exception during native engine bootstrapping initialization phase.", ex);
            }
            catch (InvalidOperationException ex)
            {
                _freeSpellChecker(_rawEngineContext);
                NativeLibrary.Free(_libraryHandle);
                throw new ExternalException("Fatal pipeline exception during native engine bootstrapping initialization phase.", ex);
            }
        }

        /// <summary>
        /// Finalizer safety net to ensure native memory blocks are systematically freed.
        /// </summary>
        ~ManagedSpellCheckService()
        {
            Dispose(false);
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Validates a targeted word string token against the underlying multi-tier unmanaged check structures.
        /// </summary>
        public SpellCheckResult VerifyWord(string word)
        {
            ObjectDisposedException.ThrowIf(_disposed, this);

            if (string.IsNullOrWhiteSpace(word))
            {
                return new SpellCheckResult(IsCorrect: false, Suggestions: Array.Empty<string>());
            }

            using var activity = _activitySource.StartActivity("Native-C++-SpellCheck", ActivityKind.Internal);
            activity?.SetTag("spellcheck.word", word);

            IntPtr rawSuggestionsPtr = IntPtr.Zero;
            IntPtr nativeWordPtr = IntPtr.Zero;

            lock (_lockRoot)
            {
                try
                {
                    // Explicitly allocate and map native string layout to isolate register stack alignment
                    nativeWordPtr = Marshal.StringToHGlobalAnsi(word);

                    // Invoke with ref pointer indirection tracking to avoid stack corruption crashes
                    int nativeResult = _checkWordABI(_rawEngineContext, nativeWordPtr, ref rawSuggestionsPtr);

                    switch (nativeResult)
                    {
                        case 1: // Word matched perfectly in lookup dictionary levels
                            return new SpellCheckResult(IsCorrect: true, Suggestions: Array.Empty<string>());

                        case 0: // Word is flag-marked misspelled
                            if (rawSuggestionsPtr == IntPtr.Zero)
                            {
                                return new SpellCheckResult(IsCorrect: false, Suggestions: Array.Empty<string>());
                            }

                            // Marshal string buffer array out of unmanaged boundaries
                            string combinedSuggestions = Marshal.PtrToStringAnsi(rawSuggestionsPtr) ?? string.Empty;
                            
                            string[] suggestionsArray = !string.IsNullOrEmpty(combinedSuggestions)
                                ? combinedSuggestions.Split('|', StringSplitOptions.RemoveEmptyEntries)
                                : Array.Empty<string>();

                            return new SpellCheckResult(IsCorrect: false, suggestionsArray);

                        case -1:
                        default:
                            throw new ExternalException($"An unhandled execution crash boundary exception state dropped inside the native component processing token '{word}'.");
                    }
                }
                catch (Exception ex)
                {
                    activity?.SetStatus(ActivityStatusCode.Error, ex.Message);
                    throw;
                }
                finally
                {
                    // Clean up the allocated unmanaged word string copy
                    if (nativeWordPtr != IntPtr.Zero)
                    {
                        Marshal.FreeHGlobal(nativeWordPtr);
                    }

                    // Safely free the buffer *only* if the native engine populated it
                    if (rawSuggestionsPtr != IntPtr.Zero)
                    {
                        _freeSuggestionsBuffer(rawSuggestionsPtr);
                    }
                }
            }
        }

        /// <summary>
        /// Commits an explicit custom validation phrase token into the native persistent store files and structural cache tracking.
        /// </summary>
        public void AddWordToPersonalDictionary(string word)
        {
            ObjectDisposedException.ThrowIf(_disposed, this);

            if (string.IsNullOrWhiteSpace(word)) return;

            lock (_lockRoot)
            {
                _insertPersonalWord(_rawEngineContext, word);
            }
        }

        #endregion

        #region Disposal Interface Implementation

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                lock (_lockRoot)
                {
                    if (!_disposed)
                    {
                        // Free the native engine instance contextual object layout allocation
                        if (_rawEngineContext != IntPtr.Zero)
                        {
                            _freeSpellChecker(_rawEngineContext);
                        }

                        // Unload the dynamic interop system pointer address descriptor handles
                        if (_libraryHandle != IntPtr.Zero)
                        {
                            NativeLibrary.Free(_libraryHandle);
                        }

                        _disposed = true;
                    }
                }
            }
        }

        #endregion
    }
}