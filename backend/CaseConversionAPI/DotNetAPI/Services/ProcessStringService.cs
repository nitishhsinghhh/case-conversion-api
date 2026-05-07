/**************************************************************************************************
 * File         : ProcessStringService.cs
 *
 * Copyright    : (c) 2016–2026 nitishhsinghh. All rights reserved.
 * This material may be reproduced for teaching and learning purposes only.
 * It is not to be used in industry or for commercial purposes.
 *
 * Class        : ProcessStringService
 *
 * Description  : Managed service wrapper for the native C++ string conversion engine.
 * Handles platform-specific library loading, symbol resolution, and
 * interop execution using P/Invoke and NativeLibrary APIs.
 *
 * Notes        : - Implements IDisposable to ensure proper release of native resources.
 * - Uses "Callee-Allocates, Caller-Frees" pattern for safe memory handling.
 * - Bridges managed (.NET) and unmanaged (C++) execution layers with OTEL.
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-11     Nitish Singh     Initial implementation of native interop layer
 * 1.1         2026-04-13     Nitish Singh     Added memory safety, freeString delegate, IDisposable
 * 1.2         2026-04-18     Nitish Singh     Added OpenTelemetry instrumentation and updated 
 * delegate for distributed tracing (traceId).
 * 1.3         2026-04-19     Nitish Singh     Added M2-Optimized Parallel Batch Processing (P-Cores)
 * 1.4         2026-04-19     Nitish Singh     Integrated ConcurrentBag and Aggregate Memory Guard
 * 1.5         2026-05-05     Nitish Singh     Fixed UTF-8 ByteCount mismatch & hardened guards.
 **************************************************************************************************/

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using System.Linq;

namespace StringConversionAPI.Services
{
    /// <summary>
    /// Service to facilitate communication between .NET and the native C++ 
    /// conversion library with built-in observability and memory safety.
    /// </summary>
    public class ProcessStringService : IDisposable
    {
        #region Performance & Security Constants

        // Hardware-targeted parallelism for M2 P-Cores to maintain maximum IPC
        private const int MaxNativeParallelism = 4;

        // Aggregate Batch Limit (20MB) to prevent container heap exhaustion
        private const long MaxBatchPayloadBytes = 5 * 1024 * 1024; 

        #endregion

        #region Native Delegates

        private delegate IntPtr ProcessStringDelegate(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string input,
            int len, 
            int choice, 
            [MarshalAs(UnmanagedType.LPUTF8Str)] string traceId);

        private delegate void FreeStringDelegate(IntPtr ptr);

        #endregion

        #region Private Members

        private readonly ProcessStringDelegate _processString;
        private readonly FreeStringDelegate _freeStringDelegate;
        private readonly IntPtr _libraryHandle;
        private bool _disposed = false;

        private static readonly ActivitySource _activitySource = new("CaseConversion.Engine");

        #endregion

        public ProcessStringService()
        {
            string dllName = RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? "ProcessStringDLL.dll" :
                             RuntimeInformation.IsOSPlatform(OSPlatform.Linux)   ? "libProcessStringDLL.so" :
                             RuntimeInformation.IsOSPlatform(OSPlatform.OSX)     ? "libProcessStringDLL.dylib" :
                             throw new PlatformNotSupportedException("Unsupported OS platform.");

            string fullPath = Path.Combine(AppContext.BaseDirectory, dllName);

            _libraryHandle = NativeLibrary.Load(fullPath);
            if (_libraryHandle == IntPtr.Zero)
                throw new DllNotFoundException($"Unable to load native library at: {fullPath}");
            
            IntPtr procAddr = NativeLibrary.GetExport(_libraryHandle, "processStringDLL");
            IntPtr freeProcAddr = NativeLibrary.GetExport(_libraryHandle, "freeString");

            _processString = Marshal.GetDelegateForFunctionPointer<ProcessStringDelegate>(procAddr);
            _freeStringDelegate = Marshal.GetDelegateForFunctionPointer<FreeStringDelegate>(freeProcAddr);   
        }

        /// <summary>
        /// Converts the input string via native engine while capturing telemetry spans.
        /// </summary>
        public string Convert(string input, int choice)
        {
            if (string.IsNullOrEmpty(input))
                return input;

            using var activity = _activitySource.StartActivity("Native-C++-Process", ActivityKind.Internal);

            // CRITICAL: Calculate actual byte count for the UTF-8 representation
            int byteCount = System.Text.Encoding.UTF8.GetByteCount(input);
            
            string traceId = activity?.Id ?? "no-trace-context";
            
            activity?.SetTag("conversion.choice", choice);

            IntPtr resultPtr = IntPtr.Zero;

            try
            {
                // Pass byteCount instead of input.Length to the native function for accurate processing and security checks
                resultPtr = _processString(input, byteCount, choice, traceId);
                
                if (resultPtr == IntPtr.Zero)
                    return string.Empty;

                string result = Marshal.PtrToStringUTF8(resultPtr) ?? string.Empty;

                // Sentinel Check for native security gate
                if (result == "ERROR_BUFFER_OVERFLOW_LIMIT_5MB")
                {
                    activity?.SetStatus(ActivityStatusCode.Error, "Input exceeds 5MB limit."); 
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
                if (resultPtr != IntPtr.Zero)
                {
                    _freeStringDelegate(resultPtr);
                }
            }
        }

        /// <summary>
        /// Hardware-optimized parallel batch processing with aggregate memory guarding.
        /// Targets the 4 Performance Cores of the M2.
        /// </summary>
        public async Task<IEnumerable<string>> ConvertBatchAsync(IEnumerable<string> inputs, int choice)
        {
            if (inputs == null) return Array.Empty<string>();

            var inputList = inputs.ToList();
            int count = inputList.Count;

            // Pre-flight Validation: Total Payload Guard
            long totalByteCount = inputList.Sum(s => (long)(s?.Length ?? 0));
            if (totalByteCount > MaxBatchPayloadBytes)
            {
                throw new ArgumentException($"Total batch payload ({totalByteCount} bytes) exceeds 5MB limit.");
            }

            // Execution Orchestration - PRESERVE ORDER & MINIMIZE CONTENTION
            var results = new string[count]; // Pre-allocated array
            var options = new ParallelOptions { MaxDegreeOfParallelism = MaxNativeParallelism };

            // Use Range to maintain index parity between input and output
            await Parallel.ForEachAsync(Enumerable.Range(0, count), options, async (i, token) =>
            {
                // Offload to Task.Run to keep P-Cores saturated
                results[i] = await Task.Run(() => Convert(inputList[i], choice), token);
            });

            return results;
        }
        #region Disposal Pattern

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

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

        ~ProcessStringService()
        {
            Dispose(false);
        }

        #endregion
    }
}