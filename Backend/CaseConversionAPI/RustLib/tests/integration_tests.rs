// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : integration_tests.rs                                */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-07                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes (isolated test execution)                      */
/* Complexity  : O(n) per conversion test                           */
/* API Status  : Stable                                              */
/* Version     : 1.0.0                                               */
/*                                                                   */
/* Description : Integration and FFI validation suite for the Rust   */
/*               string conversion engine. Covers exported C ABI,    */
/*               strategy implementations, dispatcher routing,       */
/*               error handling, memory management, stress tests,    */
/*               and conversion correctness validation.              */
/*                                                                   */
/* Test Groups :                                                     */
/*             : 1. FFI exported API tests                           */
/*             : 2. Conversion strategy tests                        */
/*             : 3. Dispatcher integration tests                     */
/*             : 4. Edge case validation                             */
/*             : 5. Invalid input testing                            */
/*             : 6. Stress and performance testing                   */
/*             : 7. Memory safety verification                       */
/*                                                                   */
/* Notes       : - Validates process_string_dll() ABI layer          */
/*             : - Ensures free_string() correctness                 */
/*             : - Uses UTF-8 byte length validation                 */
/*             : - Covers full conversion pipeline                   */
/*             : - Includes null pointer safety checks               */
/*             : - Verifies dispatcher integration                   */
/*                                                                   */
/* Exported APIs Tested:                                             */
/*             : - process_string_dll()                              */
/*             : - free_string()                                     */
/*             : - process_string()                                  */
/*                                                                   */
/* Memory Safety Notes:                                              */
/*             : - Validates CString::into_raw ownership             */
/*             : - Validates CString::from_raw cleanup               */
/*             : - Ensures null-safe free behavior                   */
/*             : - Prevents allocator mismatch across FFI            */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial integration suite  */
/*********************************************************************/

use rust_lib::ffi::{free_string, process_string_dll};

use rust_lib::strategies::StringConversion;

use std::ffi::{CStr, CString};

fn call_dll(input: &str, choice: i32) -> String {
    let input_c = CString::new(input).unwrap();
    let trace = CString::new("test-trace-id").unwrap();

    let ptr = unsafe { process_string_dll(input_c.as_ptr(), input.len(), choice, trace.as_ptr()) };

    assert!(!ptr.is_null());

    unsafe {
        let output = CStr::from_ptr(ptr).to_string_lossy().into_owned();

        free_string(ptr);

        output
    }
}

// ======================================================
// Functional Tests
// ======================================================

#[test]
fn alternating_case() {
    assert_eq!(call_dll("hello", 1), "HeLlO");
}

#[test]
fn lowercase() {
    assert_eq!(call_dll("HELLO", 3), "hello");
}

#[test]
fn uppercase() {
    assert_eq!(call_dll("hello", 4), "HELLO");
}

#[test]
fn sentence_case() {
    assert_eq!(call_dll("hello world.", 5), "Hello world.");
}

#[test]
fn reverse() {
    assert_eq!(call_dll("hello", 7), "olleh");
}

#[test]
fn remove_spaces() {
    assert_eq!(call_dll("hello world", 9), "helloworld");
}

#[test]
fn snake_case() {
    assert_eq!(call_dll("hello world", 11), "hello_world");
}

#[test]
fn kebab_case() {
    assert_eq!(call_dll("hello world", 12), "hello-world");
}

// ======================================================
// Edge Cases
// ======================================================

#[test]
fn empty_string() {
    assert_eq!(call_dll("", 4), "");
}

#[test]
fn single_character() {
    assert_eq!(call_dll("a", 4), "A");
}

#[test]
fn special_characters() {
    assert_eq!(call_dll("@#123 abc!", 4), "@#123 ABC!");
}

// ======================================================
// Invalid Inputs
// ======================================================

#[test]
fn invalid_choice() {
    let result = call_dll("hello", 999);

    assert!(result.contains("ERROR"), "Unexpected result: {}", result);
}

#[test]
fn null_input() {
    let trace = CString::new("trace").unwrap();

    let ptr = unsafe { process_string_dll(std::ptr::null(), 0, 1, trace.as_ptr()) };

    let result = unsafe {
        let value = CStr::from_ptr(ptr).to_string_lossy().into_owned();

        free_string(ptr);

        value
    };

    assert_eq!(result, "ERROR_NULL_INPUT");
}

// ======================================================
// Stress Tests
// ======================================================

#[test]
fn large_input() {
    let input = "a".repeat(10_000);

    let result = call_dll(&input, 4);

    assert_eq!(result.len(), input.len());
}

#[test]
fn multiple_calls() {
    for _ in 0..1000 {
        assert_eq!(call_dll("test", 4), "TEST");
    }
}

#[test]
fn free_null_safe() {
    unsafe {
        free_string(std::ptr::null_mut());
    }
}

use rust_lib::{
    AlternatingCaseConversion, LowerCaseConversion, ReverseConversion, ToggleCaseConversion,
    process_string,
};

#[test]
fn lowercase_conversion() {
    let strategy = LowerCaseConversion;

    assert_eq!(strategy.convert("HeLLo WoRLD!"), "hello world!");
}

#[test]
fn alternating_conversion() {
    let strategy = AlternatingCaseConversion;

    assert_eq!(strategy.convert("hello world"), "HeLlO WoRlD");
}

#[test]
fn toggle_case() {
    let strategy = ToggleCaseConversion;

    assert_eq!(strategy.convert("TeStInG"), "tEsTiNg");
}

#[test]
fn reverse_conversion() {
    let strategy = ReverseConversion;

    assert_eq!(strategy.convert("Hello"), "olleH");
}

#[test]
fn process_string_alternating() {
    let output = process_string("Hello World!", 1).unwrap();

    assert_eq!(output, "HeLlO WoRlD!");
}

#[test]
fn process_string_reverse() {
    let output = process_string("Hello World!", 7).unwrap();

    assert_eq!(output, "!dlroW olleH");
}
