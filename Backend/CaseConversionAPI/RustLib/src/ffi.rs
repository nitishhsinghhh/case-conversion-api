/*********************************************************************/
/* File        : ffi.rs                                              */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-07                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Interop                                        */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Version     : 1.0.0                                               */
/*                                                                   */
/* Description : C ABI wrapper exposing the Rust string conversion   */
/*               engine for interoperability with C, C++, C#, and    */
/*               other foreign language runtimes.                    */
/*                                                                   */
/*               Delegates requests to the dispatcher layer, which   */
/*               performs conversion type validation, strategy       */
/*               creation, and execution.                            */
/*                                                                   */
/* Exported APIs:                                                    */
/*             : - process_string_dll()                              */
/*             : - free_string()                                     */
/*                                                                   */
/* Notes       : - Stable C ABI using extern "C"                     */
/*             : - Designed for .NET P/Invoke interoperability       */
/*             : - Enforces strict 5MB input size limit              */
/*             : - Uses CString allocation for ABI safety            */
/*             : - Caller owns returned memory                       */
/*             : - Returned memory must be released using            */
/*                 free_string()                                     */
/*             : - Panic-safe boundary via catch_unwind()            */
/*             : - Returns standardized error strings               */
/*             : - TraceId reserved for future observability         */
/*             : - UTF-8 validation enforced before processing       */
/*                                                                   */
/* Memory Safety Notes:                                              */
/*             : - Ownership transferred through                    */
/*                 CString::into_raw()                               */
/*             : - Memory reclaimed through                          */
/*                 CString::from_raw()                               */
/*             : - No mixed allocator usage across boundaries        */
/*             : - Panic isolation prevents unwinding across ABI     */
/*             : - Null-pointer checks performed before dereference  */
/*                                                                   */
/* Error Codes:                                                      */
/*             : - ERROR_NULL_INPUT                                  */
/*             : - ERROR_BUFFER_OVERFLOW_LIMIT_5MB                   */
/*             : - ERROR_NEGATIVE_CONVERSION_CHOICE                  */
/*             : - ERROR_INVALID_CONVERSION_CHOICE                   */
/*             : - ERROR_INVALID_UTF8                                */
/*             : - ERROR_STRING_CONTAINS_NULL                        */
/*             : - ERROR_INTERNAL_EXCEPTION                          */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial Rust FFI layer     */
/* 1.1.0      2026-06-07  Nitish Singh    Added UTF-8 validation     */
/* 1.2.0      2026-06-07  Nitish Singh    Added panic isolation      */
/* 1.3.0      2026-06-07  Nitish Singh    Added 5MB safety limit     */
/* 1.4.0      2026-06-07  Nitish Singh    Added traceId parameter    */
/*********************************************************************/

use crate::dispatcher::process_string;

use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use std::panic;

//===================================================================
// Constants: 5 MB Buffer Limit
//===================================================================

const MAX_INPUT_SIZE: usize = 5 * 1024 * 1024;

//===================================================================
// Helper Utilities (Internal Only)
//===================================================================

fn allocate_c_string(value: &str) -> *mut c_char {
    match CString::new(value) {
        Ok(s) => s.into_raw(),
        Err(_) => CString::new("ERROR_STRING_CONTAINS_NULL")
            .unwrap()
            .into_raw(),
    }
}

fn safe_error(message: &str) -> *mut c_char {
    allocate_c_string(message)
}

//===================================================================
// Exported DLL API (Extern "C")
//===================================================================

/// # Safety
///
/// This function is part of a C ABI (FFI) boundary and accepts raw pointers
/// from external callers. The caller must ensure:
///
/// - `input` points to a valid UTF-8 buffer of length `len`
/// - `trace_id` is either null or a valid null-terminated C string
/// - Pointers remain valid for the duration of the call
/// - Memory ownership rules are respected (see module documentation)
#[unsafe(no_mangle)]
pub unsafe extern "C" fn process_string_dll(
    input: *const c_char,
    len: usize,
    choice: i32,
    trace_id: *const c_char,
) -> *mut c_char {
    let result = panic::catch_unwind(|| {
        //-----------------------------------------------------------
        // Null Input Check
        //-----------------------------------------------------------

        if input.is_null() {
            return safe_error("ERROR_NULL_INPUT");
        }

        //-----------------------------------------------------------
        // Length Validation
        //-----------------------------------------------------------

        if len > MAX_INPUT_SIZE {
            return safe_error("ERROR_BUFFER_OVERFLOW_LIMIT_5MB");
        }

        //-----------------------------------------------------------
        // Choice Validation
        //-----------------------------------------------------------

        if choice < 0 {
            return safe_error("ERROR_NEGATIVE_CONVERSION_CHOICE");
        }

        //-----------------------------------------------------------
        // Convert Raw Buffer -> UTF-8 String
        //-----------------------------------------------------------

        let bytes = unsafe { std::slice::from_raw_parts(input as *const u8, len) };

        let input_str = match std::str::from_utf8(bytes) {
            Ok(v) => v,
            Err(_) => {
                return safe_error("ERROR_INVALID_UTF8");
            }
        };

        //-----------------------------------------------------------
        // Optional TraceId
        //-----------------------------------------------------------

        let _trace_id = if !trace_id.is_null() {
            unsafe { CStr::from_ptr(trace_id).to_string_lossy().into_owned() }
        } else {
            String::new()
        };

        //-----------------------------------------------------------
        // Execute Conversion Pipeline
        //-----------------------------------------------------------

        match process_string(input_str, choice) {
            Ok(output) => allocate_c_string(&output),

            Err(error) => safe_error(error),
        }
    });

    //---------------------------------------------------------------
    // Panic Protection
    //---------------------------------------------------------------

    match result {
        Ok(ptr) => ptr,

        Err(_) => safe_error("ERROR_INTERNAL_EXCEPTION"),
    }
}

//===================================================================
// Memory Release API
//===================================================================

/// # Safety
///
/// This function frees memory previously allocated by `process_string_dll`.
///
/// # Caller guarantees
/// - `ptr` must be either:
///   - a null pointer (safe no-op), OR
///   - a pointer returned by `CString::into_raw()` from this library
///
/// # Undefined behavior
/// - Passing a pointer not allocated by this library
/// - Passing a pointer that has already been freed
#[unsafe(no_mangle)]
pub unsafe extern "C" fn free_string(ptr: *mut c_char) {
    if ptr.is_null() {
        return;
    }

    unsafe {
        drop(CString::from_raw(ptr));
    }
}
