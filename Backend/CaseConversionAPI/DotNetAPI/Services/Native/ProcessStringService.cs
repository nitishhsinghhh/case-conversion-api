/**************************************************************************************************
 * File         : ProcessStringService.cs
 *
 * Copyright    : (c) 2016–2026 Nitish Singh. All rights reserved.
 * License      : Licensed under the Apache License, Version 2.0 (the "License");
 *                you may not use this file except in compliance with the License.
 *                You may obtain a copy of the License at
 *
 *                http://www.apache.org/licenses/LICENSE-2.0
 *
 *                Unless required by applicable law or agreed to in writing, software
 *                distributed under the License is distributed on an "AS IS" BASIS,
 *                WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *                See the License for the specific language governing permissions and
 *                limitations under the License.
 *
 * Description  : Managed service wrapper facilitating platform-agnostic communication with the
 *                native unmanaged C++ string conversion engine. Orchestrates dynamic library loading,
 *                hardware-optimized parallel batch processing, memory guards, and cross-boundary telemetry.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-11     Nitish Singh     Initial implementation of native interop layer using P/Invoke.
 * 1.1         2026-04-13     Nitish Singh     Introduced explicit unmanaged memory safety architecture via
 *                                             the Callee-Allocates, Caller-Frees pattern and IDisposable.
 * 1.2         2026-04-18     Nitish Singh     Integrated OpenTelemetry ActivitySource instrumentation and 
 *                                             distributed tracing traceId contextual propagation across 
 *                                             runtime boundaries.
 * 1.3         2026-04-19     Nitish Singh     Engineered Apple M2 P-Core targeted parallel batch processing 
 *                                             routines.
 * 1.4         2026-04-19     Nitish Singh     Hardened concurrency using pre-allocated collections and 
 *                                             added systemic aggregate unmanaged memory heap exhaustion guards.
 * 1.5         2026-05-05     Nitish Singh     Resolved data truncation bug by fixing multi-byte UTF-8 
 *                                             ByteCount mismatches and refactored parallel orchestration 
 *                                             routines for strict element order preservation.
 **************************************************************************************************/

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace StringConversionAPI.Services
{
    /// <summary>
    /// Provides low-latency, hardware-optimized orchestration between the managed .NET runtime and 
    /// the unmanaged native C++ execution layer. Implements automated garbage collection disposal patterns 
    /// for native system descriptors and locks parallel work to specific performance-core limits.
    /// </summary>
    public class ProcessStringService : IDisposable
    {
        #region Performance & Security Constants

        /// <summary>
        /// Defines the optimum physical execution width targeting specific hardware architectures (e.g., Apple M2 Performance Cores) 
        /// to maximize Instruction Per Cycle (IPC) throughput while minimizing cache thrashing.
        /// </summary>
        private const int MaxNativeParallelism = 4;

        /// <summary>
        /// Defines the rigid maximum allocation threshold (5 MB in bytes) allowed for aggregate processing payloads 
        /// to mitigate systemic unmanaged out-of-memory vulnerabilities or host container crash vectors.
        /// </summary>
        private const long MaxBatchPayloadBytes = 5 * 1024 * 1024;

        #endregion

        #region Native Function Pointers & Delegates

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private delegate IntPtr ProcessStringDelegate(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string input,
            int len,
            int choice,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string traceId);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void FreeStringDelegate(IntPtr ptr);

        #endregion

        #region Private Fields

        private readonly ProcessStringDelegate _processString;
        private readonly FreeStringDelegate _freeStringDelegate;
        private readonly IntPtr _libraryHandle;
        private bool _disposed;

        private static readonly ActivitySource _activitySource = new("CaseConversion.Engine");

        #endregion

        #region Constructors / Finalizers

        /// <summary>
        /// Initializes a new instance of the <see cref="ProcessStringService"/> class.
        /// Dynamically resolves and links OS-specific binary dependencies at application runtime.
        /// </summary>
        /// <exception cref="PlatformNotSupportedException">Thrown when operating on unmapped OS environments.</exception>
        /// <exception cref="DllNotFoundException">Thrown when the target unmanaged module cannot be resolved within path scopes.</exception>
        public ProcessStringService()
        {
            string dllName = RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? "libProcessStringDLL.dll" :
                             RuntimeInformation.IsOSPlatform(OSPlatform.Linux)   ? "libProcessStringDLL.so" :
                             RuntimeInformation.IsOSPlatform(OSPlatform.OSX)     ? "libProcessStringDLL.dylib" :
                             throw new PlatformNotSupportedException("The executing operating system platform is not supported.");

            string fullPath = Path.Combine(AppContext.BaseDirectory, dllName);

            _libraryHandle = NativeLibrary.Load(fullPath);
            if (_libraryHandle == IntPtr.Zero)
            {
                throw new DllNotFoundException($"Unable to load native boundary engine library at path resource: {fullPath}");
            }

            IntPtr procAddr = NativeLibrary.GetExport(_libraryHandle, "processStringDLL");
            IntPtr freeProcAddr = NativeLibrary.GetExport(_libraryHandle, "freeString");

            _processString = Marshal.GetDelegateForFunctionPointer<ProcessStringDelegate>(procAddr);
            _freeStringDelegate = Marshal.GetDelegateForFunctionPointer<FreeStringDelegate>(freeProcAddr);
        }

        /// <summary>
        /// Finalizes an instance of the <see cref="ProcessStringService"/> class.
        /// Acts as a safety net fallback to guarantee that unmanaged system handles are reclaimed.
        /// </summary>
        ~ProcessStringService()
        {
            Dispose(false);
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Executes a single text case transformation across the unmanaged runtime interface boundaries.
        /// Tracks execution context using distributed OpenTelemetry tracing parameters.
        /// </summary>
        /// <param name="input">The target managed source string requiring mutation processing.</param>
        /// <param name="choice">The specific functional conversion algorithmic index to be executed.</param>
        /// <returns>The fully processed string returned from the native compiler engine layer.</returns>
        public string Convert(string input, int choice)
        {
            if (string.IsNullOrEmpty(input))
            {
                return input;
            }

            using var activity = _activitySource.StartActivity("Native-C++-Process", ActivityKind.Internal);

            // Calculate precise multi-byte boundary limits for UTF-8 compatibility to prevent truncation across marshaling steps.
            int byteCount = System.Text.Encoding.UTF8.GetByteCount(input);
            string traceId = activity?.Id ?? "no-trace-context";

            activity?.SetTag("conversion.choice", choice);
            IntPtr resultPtr = IntPtr.Zero;

            try
            {
                // Pass byteCount instead of string length to ensure memory-accurate pointer sizing within unmanaged buffers
                resultPtr = _processString(input, byteCount, choice, traceId);

                if (resultPtr == IntPtr.Zero)
                {
                    return string.Empty;
                }

                string result = Marshal.PtrToStringUTF8(resultPtr) ?? string.Empty;

                // Validate engine security boundaries for internal errors passed via predefined string tokens
                if (result == "ERROR_BUFFER_OVERFLOW_LIMIT_5MB")
                {
                    activity?.SetStatus(ActivityStatusCode.Error, "The provided string allocation block size exceeded the internal 5MB memory guard limit.");
                }

                return result;
            }
            catch (Exception ex)
            {
                activity?.SetStatus(ActivityStatusCode.Error, ex.Message);
                throw;
            }
            finally
            {
                // Enforce caller-frees design contract to prevent memory leaks in the application process space
                if (resultPtr != IntPtr.Zero)
                {
                    _freeStringDelegate(resultPtr);
                }
            }
        }

        /// <summary>
        /// Performs asynchronous parallel batch processing on a sequence of inputs, targeting local performance core topologies.
        /// Guarantees index ordering preservation between request arrays and returned result sets.
        /// </summary>
        /// <param name="inputs">The collection of payload strings to submit for rapid processing.</param>
        /// <param name="choice">The specific algorithmic modification identifier to be globally applied.</param>
        /// <returns>An ordered collection containing the modified output payloads.</returns>
        /// <exception cref="ArgumentException">Thrown if the cumulative raw payload sizes violate security limits.</exception>
        public async Task<IEnumerable<string>> ConvertBatchAsync(IEnumerable<string> inputs, int choice)
        {
            if (inputs == null)
            {
                return Array.Empty<string>();
            }

            List<string> inputList = inputs.ToList();
            int count = inputList.Count;

            // Enforce explicit size validation boundaries prior to scheduling tasks to minimize unmanaged overhead risks
            long totalByteCount = inputList.Sum(s => (long)(s?.Length ?? 0));
            if (totalByteCount > MaxBatchPayloadBytes)
            {
                throw new ArgumentException($"The cumulative size of the submitted batch payload ({totalByteCount} bytes) violates the maximum security threshold of {MaxBatchPayloadBytes} bytes.");
            }

            string[] results = new string[count];
            ParallelOptions options = new() { MaxDegreeOfParallelism = MaxNativeParallelism };

            // Process via deterministic zero-allocation range looping to ensure elements match request index spots perfectly
            await Parallel.ForEachAsync(Enumerable.Range(0, count), options, async (i, token) =>
            {
                results[i] = await Task.Run(() => Convert(inputList[i], choice), token);
            });

            return results;
        }

        #endregion

        #region Disposal Interface Implementation

        /// <summary>
        /// Releases all operational resource handles currently requested by the interop management layer.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Contextual handler invoked to selectively clean up tracking state blocks inside native components.
        /// </summary>
        /// <param name="disposing">True to release both managed and unmanaged dependencies; false to release only unmanaged pointers.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_libraryHandle != IntPtr.Zero)
                {
                    NativeLibrary.Free(_libraryHandle);
                }

                _disposed = true;
            }
        }

        #endregion
    }
}