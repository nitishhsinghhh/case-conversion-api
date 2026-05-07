// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : StringConversionLoggingTests.cpp                    */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : No (test suite)                                     */
/* Complexity  : O(n) per test case                                  */
/* API Status  : Stable                                              */
/* Exception Safety : N/A (test environment)                         */
/*                                                                   */
/* Description : Google Test suite validating string conversion      */
/*               strategies with logging support. Tests include      */
/*               advanced conversions, client strategy execution,    */
/*               processString integration, and reverse conversion.  */
/*                                                                   */
/* Test Groups :                                                     */
/*               1. Advanced conversions with logging                */
/*               2. Client strategy pattern tests                    */
/*               3. processString integration tests                  */
/*               4. Special conversion tests                         */
/*               5. Performance validation tests                     */
/*               6. Fuzz testing with random inputs                  */
/*                                                                   */
/* Notes       : - Uses TestHelpers::logConversion for trace output  */
/*             : - Validates correctness across all strategies       */
/*             : - Includes performance and fuzz coverage            */
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
#include "ConversionResult.hpp"
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

#include <chrono>
#include <iostream>
#include <random>

// ============================================================
// 1. ADVANCED CONVERSION TESTS (WITH LOGGING)
// ============================================================

TEST(AdvancedConversionTest, MixedCaseInputWithLog) {
  std::string input = "hElLo WoRLd!";

  LowerCaseConversion lower;
  UpperCaseConversion upper;
  CapitalizeWordsConversion cap;
  SentenceCaseConversion sentence;
  ToggleCaseConversion toggle;
  AlternatingCaseConversion alternating;

  std::string result;

  result = ConversionResult(lower.convert(input)).get_c_str();
  logConversion("LowerCase", input, result);
  EXPECT_EQ(result, "hello world!");

  result = ConversionResult(upper.convert(input)).get_c_str();
  logConversion("UpperCase", input, result);
  EXPECT_EQ(result, "HELLO WORLD!");

  result = ConversionResult(cap.convert(input)).get_c_str();
  logConversion("CapitalizeWords", input, result);
  EXPECT_EQ(result, "Hello World!");

  result = ConversionResult(sentence.convert(input)).get_c_str();
  logConversion("SentenceCase", input, result);
  EXPECT_EQ(result, "Hello world!");

  result = ConversionResult(toggle.convert(input)).get_c_str();
  logConversion("ToggleCase", input, result);
  EXPECT_EQ(result, "HeLlO wOrlD!");

  result = ConversionResult(alternating.convert(input)).get_c_str();
  logConversion("AlternatingCase", input, result);
  EXPECT_EQ(result, "HeLlO WoRlD!");
}

// ============================================================
// 2. CLIENT (STRATEGY PATTERN) TESTS
// ============================================================

TEST(ClientTest, ExecuteStrategyWithLog) {
  std::string input = "TeStInG";

  Client client;
  client.setStrategy(StringConversionFactory::create(ConversionType::Toggle));

  std::string output = ConversionResult(client.execute(input)).get_c_str();

  std::cout << "[Client Toggle] Input: \"" << input << "\" => Output: \""
            << output << "\"" << std::endl;

  EXPECT_EQ(output, "tEsTiNg");
}

// ============================================================
// 3. PROCESS STRING (INTEGRATION TEST)
// ============================================================

TEST(ProcessStringTest, ProcessStringAlternating) {
  std::string input = "Hello World!";
  int choice = 1; // Alternating case

  std::string output = ConversionResult(processString(input, choice)).get_c_str();

  logConversion("ProcessString Alternating", input, output);

  EXPECT_EQ(output, "HeLlO WoRlD!");
}

TEST(ProcessStringTest, ProcessStringReverse) {
  std::string input = "Hello World!";
  int choice = 7; // Reverse

  std::string output = ConversionResult(processString(input, choice)).get_c_str();

  logConversion("ProcessString Reverse", input, output);

  EXPECT_EQ(output, "!dlroW olleH");
}

// ============================================================
// 4. SPECIAL CONVERSION TESTS
// ============================================================

TEST(ReverseConversionTest, ReverseStringWithLog) {
  std::string input = "Hello World!";
  ReverseConversion reverse;

  std::string output = ConversionResult(reverse.convert(input)).get_c_str();

  logConversion("ReverseCase", input, output);

  EXPECT_EQ(output, "!dlroW olleH");
}

// ============================================================
// 5. PERFORMANCE TEST
// ============================================================

TEST(UpperCasePerformanceTest, LargeInput) {
  UpperCaseConversion converter;

  std::string largeInput(1'000'000, 'a');

  auto start = std::chrono::high_resolution_clock::now();
  auto resultObj = converter.convert(largeInput); 
  const char* resultStr = resultObj.get_c_str();
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Execution time: " << duration.count() << " ms\n";

  EXPECT_EQ(std::strlen(resultStr), largeInput.size());

  EXPECT_LT(duration.count(), 25); // < 25 ms
}

// ============================================================
// 6. FUZZ TESTING (RANDOM INPUTS)
// ============================================================

TEST(UpperCaseFuzzTest, RandomStrings) {
  UpperCaseConversion converter;

  std::mt19937 rng(123);
  std::uniform_int_distribution<int> lenDist(0, 100);
  std::uniform_int_distribution<int> charDist(32, 126);

  for (int i = 0; i < 1000; ++i) {
    int len = lenDist(rng);
    std::string input;

    for (int j = 0; j < len; ++j) {
      input += static_cast<char>(charDist(rng));
    }

    std::string output = ConversionResult(converter.convert(input)).get_c_str();

    EXPECT_EQ(output.size(), input.size());
  }
}