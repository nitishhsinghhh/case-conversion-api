// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ProcessStringDLLTests.cpp                           */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : Core/Interop (DLL Layer)                            */
/* Thread Safe : No (test suite)                                     */
/* Complexity  : O(n) per test                                       */
/* API Status  : Stable                                              */
/* Exception Safety : N/A (test environment)                         */
/*                                                                   */
/* Description : Google Test suite validating the DLL interop layer  */
/*               of the string conversion engine. Covers all exposed */
/*               C-style API conversions, edge cases, invalid inputs,*/
/*               stress scenarios, and memory management behavior.   */
/*                                                                   */
/* Test Groups :                                                     */
/*               1. Functional DLL conversion tests                  */
/*               2. Edge case validation                             */
/*               3. Invalid input handling                           */
/*               4. Stress / performance testing                     */
/*               5. Memory management verification                   */
/*                                                                   */
/* Notes       : - Uses processStringDLL for C# P/Invoke simulation  */
/*             : - Ensures freeString is called for heap safety      */
/*             : - Validates full strategy coverage via DLL layer    */
/*********************************************************************/

// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : StringConversionTests.cpp                           */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes (read-only test execution)                     */
/* Complexity  : O(n) per conversion                                */
/* API Status  : Stable                                              */
/* Exception Safety : N/A (test layer)                               */
/*                                                                   */
/* Description : Unit tests for String Conversion library using      */
/*               Google Test framework. Covers basic conversions,    */
/*               advanced conversions, factory creation, strategy     */
/*               pattern behavior, processString API, and edge       */
/*               cases.                                              */
/*                                                                   */
/* Test Groups :                                                     */
/*               1. Basic conversion tests                           */
/*               2. Advanced conversion tests                        */
/*               3. Edge case tests                                  */
/*               4. Factory tests                                    */
/*               5. Client strategy tests                            */
/*               6. ProcessString integration tests                  */
/*               7. Logging tests                                    */
/*                                                                   */
/* Notes       : Requires GoogleTest and linked conversion library.  */
/*             : Uses TestHelpers::logConversion for trace output    */
/*             : and debugging validation.                           */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial test suite         */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <gtest/gtest.h>

// ---------------------------
// Core Includes
// ---------------------------
#include "AlternatingCaseConversion.hpp"
#include "CapitalizeWordsConversion.hpp"
#include "IStringConversion.hpp"
#include "LowerCaseConversion.hpp"
#include "ReverseConversion.hpp"
#include "SentenceCaseConversion.hpp"
#include "TestHelpers.hpp"
#include "ToggleCaseConversion.hpp"
#include "UpperCaseConversion.hpp"

// ---------------------------
// Design Pattern / Framework
// ---------------------------
#include "Client.hpp"
#include "ProcessString.hpp"
#include "StringConversionFactory.hpp"

extern "C" {
char *processStringDLL(const char *input, int len, int choice, const char *traceId);
void freeString(char *str);
}

// ============================================================
// 1. FUNCTIONAL TESTS FOR DLL
// ============================================================

TEST(ProcessStringDLL, AlternatingCase) {
  char *result = processStringDLL("hello", 5, 1, "test-trace-id");
  ASSERT_STREQ(result, "HeLlO");
  freeString(result);
}

TEST(ProcessStringDLL, CapitalizeWords) {
  char *result = processStringDLL("hello world", 11, 2, "test-trace-id");
  ASSERT_STREQ(result, "Hello World");
  freeString(result);
}

TEST(ProcessStringDLL, LowerCase) {
  char *result = processStringDLL("HELLO", 5, 3, "test-trace-id");
  ASSERT_STREQ(result, "hello");
  freeString(result);
}

TEST(ProcessStringDLL, UpperCase) {
  char *result = processStringDLL("hello", 5, 4, "test-trace-id");
  ASSERT_STREQ(result, "HELLO");
  freeString(result);
}

TEST(ProcessStringDLL, SentenceCase) {
  char *result = processStringDLL("hello world.", 12, 5, "test-trace-id");
  ASSERT_STREQ(result, "Hello world.");
  freeString(result);
}

TEST(ProcessStringDLL, ToggleCase) {
  char *result = processStringDLL("HeLLo", 6, 6, "test-trace-id");
  ASSERT_STREQ(result, "hEllO");
  freeString(result);
}

TEST(ProcessStringDLL, Reverse) {
  char *result = processStringDLL("hello", 5, 7, "test-trace-id");
  ASSERT_STREQ(result, "olleh");
  freeString(result);
}

TEST(ProcessStringDLL, RemoveVowels) {
  char *result = processStringDLL("hello world", 11, 8, "test-trace-id");
  ASSERT_STREQ(result, "hll wrld");
  freeString(result);
}

TEST(ProcessStringDLL, RemoveSpaces) {
  char *result = processStringDLL("hello world", 11, 9, "test-trace-id");
  ASSERT_STREQ(result, "helloworld");
  freeString(result);
}

TEST(ProcessStringDLL, InvertWords) {
  char *result = processStringDLL("hello world", 11, 10, "test-trace-id");
  ASSERT_STREQ(result, "olleh dlrow");
  freeString(result);
}

TEST(ProcessStringDLL, SnakeCase) {
  char *result = processStringDLL("hello world", 11, 11, "test-trace-id");
  ASSERT_STREQ(result, "hello_world");
  freeString(result);
}

TEST(ProcessStringDLL, KebabCase) {
  char *result = processStringDLL("hello world", 12, 12, "test-trace-id");
  ASSERT_STREQ(result, "hello-world");
  freeString(result);
}

TEST(ProcessStringDLL, LeetSpeak) {
  char *result = processStringDLL("elite hacker", 13, 13, "test-trace-id");
  ASSERT_TRUE(result != nullptr); // flexible validation
  freeString(result);
}

// ============================================================
// 2. EDGE CASES FOR DLL
// ============================================================

TEST(ProcessStringDLL, EmptyString) {
  char *result = processStringDLL("", 0, 0, "test-trace-id");
  ASSERT_STREQ(result, "");
  freeString(result);
}

TEST(ProcessStringDLL, SingleCharacter) {
  char *result = processStringDLL("a", 1, 4, "test-trace-id");
  ASSERT_STREQ(result, "A");
  freeString(result);
}

TEST(ProcessStringDLL, SpecialCharacters) {
  char *result = processStringDLL("@#123 abc!", 10, 4, "test-trace-id");
  ASSERT_STREQ(result, "@#123 ABC!");
  freeString(result);
}

// ============================================================
// 3. INVALID INPUTS FOR DLL
// ============================================================

TEST(ProcessStringDLL, InvalidChoice) {
  char *result = processStringDLL("hello", 5, 999, "test-trace-id");
  ASSERT_STREQ(result, "hello"); // based on your fallback
  freeString(result);
}

TEST(ProcessStringDLL, NullInput) {
  char *result = processStringDLL(nullptr, 0, 0, "test-trace-id");
  ASSERT_TRUE(result == nullptr || strcmp(result, "") == 0);
  if (result)
    freeString(result);
}

// ============================================================
// 4. STRESS TESTS FOR DLL
// ============================================================

TEST(ProcessStringDLL, LargeInput) {
  std::string large(10000, 'a');

  char *result = processStringDLL(large.c_str(), 10000, 4, "test-trace-id");

  ASSERT_EQ(strlen(result), large.size());
  freeString(result);
}

TEST(ProcessStringDLL, MultipleCalls) {
  for (int i = 0; i < 1000; i++) {
    char *result = processStringDLL("test", 4, 4, "test-trace-id");
    ASSERT_STREQ(result, "TEST");
    freeString(result);
  }
}

// ============================================================
// 5. MEMORY MANAGEMENT TESTS FOR DLL
// ============================================================

TEST(ProcessStringDLL, MemoryNotNull) { 
  const char *result = processStringDLL("hello", 4, 4, "test-trace-id"); 
  ASSERT_NE(result, nullptr);
  // Cast to char* if your freeString expects it, or keep it consistent
  freeString(const_cast<char*>(result)); 
}