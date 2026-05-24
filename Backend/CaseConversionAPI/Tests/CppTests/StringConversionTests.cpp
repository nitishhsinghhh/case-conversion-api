/*********************************************************************/
/* File: StringConversionTests.cpp                                   */
/*                                                                   */
/* Copyright (c) 2016-2026 nitishhsinghh. All rights reserved.       */
/* This material may be reproduced for teaching and learning         */
/* purposes only. It is not to be used in industry or for            */
/* commercial purposes.                                              */
/*                                                                   */
/* Description : Unit tests for String Conversion library using      */
/*               Google Test framework. Covers basic conversions,    */
/*               advanced conversions, factory creation, strategy    */
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
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <gtest/gtest.h>
#include <iostream>

// Core Interfaces
#include "Client.hpp"
#include "IStringConversion.hpp"
#include "ProcessString.hpp"
#include "StringConversionFactory.hpp"
#include "TestHelpers.hpp"
#include "ConversionResult.hpp"

// Basic Conversions
#include "AlternatingCaseConversion.hpp"
#include "CapitalizeWordsConversion.hpp"
#include "LowerCaseConversion.hpp"
#include "ReverseConversion.hpp"
#include "SentenceCaseConversion.hpp"
#include "ToggleCaseConversion.hpp"
#include "UpperCaseConversion.hpp"

// Advanced Conversions
#include "InvertWordsConversion.hpp"
#include "KebabCaseConversion.hpp"
#include "LeetSpeakConversion.hpp"
#include "RemoveSpacesConversion.hpp"
#include "RemoveVowelsConversion.hpp"
#include "SnakeCaseConversion.hpp"

// Enmum for conversion choices
#include "ConversionTypeEnum.hpp"

//
// ======================================================
// 1. BASIC CONVERSION TESTS
// ======================================================
//

TEST(LowerCaseConversionTest, Basic) {
  LowerCaseConversion conv;
  EXPECT_STREQ(conv.convert("HELLO").get_c_str(), "hello");
}

TEST(UpperCaseConversionTest, Basic) {
  UpperCaseConversion conv;
  EXPECT_STREQ(conv.convert("hello").get_c_str(), "HELLO");
}

TEST(CapitalizeWordsConversionTest, Basic) {
  CapitalizeWordsConversion conv;
  EXPECT_STREQ(conv.convert("hello world").get_c_str(), "Hello World");
}

TEST(SentenceCaseConversionTest, Basic) {
  SentenceCaseConversion conv;
  EXPECT_STREQ(conv.convert("hELLO WORLD").get_c_str(), "Hello world");
}

TEST(ToggleCaseConversionTest, Basic) {
  ToggleCaseConversion conv;
  EXPECT_STREQ(conv.convert("HeLLo").get_c_str(), "hEllO");
}

TEST(AlternatingCaseConversionTest, Basic) {
  AlternatingCaseConversion conv;
  EXPECT_STREQ(conv.convert("hello world").get_c_str(), "HeLlO WoRlD");
}

TEST(ReverseConversionTest, Basic) {
  ReverseConversion conv;
  EXPECT_STREQ(conv.convert("Hello").get_c_str(), "olleH");
  EXPECT_STREQ(conv.convert("Hello World!").get_c_str(), "!dlroW olleH");
}

//
// ======================================================
// 2. ADVANCED CONVERSION TESTS
// ======================================================
//

TEST(InvertWordsConversionTest, Basic) {
  InvertWordsConversion conv;
  EXPECT_STREQ(conv.convert("Hello World").get_c_str(), "olleH dlroW");
}

TEST(KebabCaseConversionTest, Basic) {
  KebabCaseConversion conv;
  EXPECT_STREQ(conv.convert("Hello World Example").get_c_str(), "hello-world-example");
}

TEST(SnakeCaseConversionTest, Basic) {
  SnakeCaseConversion conv;
  EXPECT_STREQ(conv.convert("Hello World Example").get_c_str(), "hello_world_example");
}

TEST(RemoveSpacesConversionTest, Basic) {
  RemoveSpacesConversion conv;
  EXPECT_STREQ(conv.convert("Hello World").get_c_str(), "HelloWorld");
}

TEST(RemoveVowelsConversionTest, Basic) {
  RemoveVowelsConversion conv;
  EXPECT_STREQ(conv.convert("Hello World").get_c_str(), "Hll Wrld");
}

TEST(LeetSpeakConversionTest, Basic) {
  LeetSpeakConversion conv;
  EXPECT_STREQ(conv.convert("Hello").get_c_str(), "H3ll0");
  EXPECT_STREQ(conv.convert("Testing").get_c_str(),
            "73571ng"); // ensure mapping matches implementation
}

//
// ======================================================
// 3. EDGE CASE TESTS
// ======================================================
//

TEST(EdgeCases, EmptyString) {
  LowerCaseConversion conv;
  EXPECT_STREQ(conv.convert("").get_c_str(), "");
}

TEST(EdgeCases, OnlySpaces) {
  RemoveSpacesConversion conv;
  EXPECT_STREQ(conv.convert("   ").get_c_str(), "");
}

TEST(EdgeCases, NumbersOnly) {
  UpperCaseConversion conv;
  EXPECT_STREQ(conv.convert("12345").get_c_str(), "12345");
}

TEST(EdgeCases, SpecialCharacters) {
  ReverseConversion conv;
  EXPECT_STREQ(conv.convert("!@#$").get_c_str(), "$#@!");
}

TEST(EdgeCases, OnlyVowels) {
  RemoveVowelsConversion conv;
  EXPECT_STREQ(conv.convert("aeiouAEIOU").get_c_str(), "");
}

TEST(EdgeCases, NoVowels) {
  RemoveVowelsConversion conv;
  EXPECT_STREQ(conv.convert("bcdfg").get_c_str(), "bcdfg");
}

TEST(LeetSpeakConversionTest, EdgeCases) {
  LeetSpeakConversion conv;
  EXPECT_STREQ(conv.convert("").get_c_str(), "");
  EXPECT_STREQ(conv.convert("123").get_c_str(), "123");
}

//
// ======================================================
// 4. FACTORY TESTS
// ======================================================
//

TEST(FactoryTest, CreatesLowerCase) {
  auto conv = StringConversionFactory::create(ConversionType::Lower);
  ASSERT_NE(conv, nullptr);
  EXPECT_STREQ(conv->convert("HELLO").get_c_str(), "hello");
}

TEST(FactoryTest, CreatesUpperCase) {
  auto conv = StringConversionFactory::create(ConversionType::Upper);
  ASSERT_NE(conv, nullptr);
  EXPECT_STREQ(conv->convert("hello").get_c_str(), "HELLO");
}

TEST(FactoryTest, CreatesReverse) {
  auto conv = StringConversionFactory::create(ConversionType::Reverse);
  ASSERT_NE(conv, nullptr);
  EXPECT_STREQ(conv->convert("Hello").get_c_str(), "olleH");
}

//
// ======================================================
// 5. CLIENT (STRATEGY PATTERN) TESTS
// ======================================================
//

TEST(ClientTest, ExecutesStrategy) {
  Client client;

  client.setStrategy(StringConversionFactory::create(ConversionType::Lower));
  EXPECT_STREQ(client.execute("HELLO").get_c_str(), "hello");

  client.setStrategy(StringConversionFactory::create(ConversionType::Toggle));
  EXPECT_STREQ(client.execute("AbC").get_c_str(), "aBc");

  client.setStrategy(StringConversionFactory::create(ConversionType::Reverse));
  EXPECT_STREQ(client.execute("Hello").get_c_str(), "olleH");
}

TEST(ClientTest, NoStrategySet) {
  Client client;
  EXPECT_STREQ(client.execute("Hello").get_c_str(), "Hello");
}

//
// ======================================================
// 6. PROCESS STRING TESTS
// ======================================================
//

TEST(ProcessStringTest, BasicFlow) {
  EXPECT_STREQ(processString("hello world",
                          static_cast<int>(ConversionChoice::Alternating)).get_c_str(),
            "HeLlO WoRlD");
  EXPECT_STREQ(processString("hello world",
                          static_cast<int>(ConversionChoice::Capitalize)).get_c_str(),
            "Hello World");
  EXPECT_STREQ(processString("Hello", static_cast<int>(ConversionChoice::Lower)).get_c_str(),
            "hello");
  EXPECT_STREQ(processString("Hello", static_cast<int>(ConversionChoice::Upper)).get_c_str(),
            "HELLO");
  EXPECT_STREQ(processString("hELLO wORLD",
                          static_cast<int>(ConversionChoice::Sentence)).get_c_str(),
            "Hello world");
  EXPECT_STREQ(processString("HeLLo", static_cast<int>(ConversionChoice::Toggle)).get_c_str(),
            "hEllO");
  EXPECT_STREQ(processString("Hello", static_cast<int>(ConversionChoice::Reverse)).get_c_str(),
            "olleH");
}

TEST(ProcessStringTest, AdvancedChoices) {
  EXPECT_STREQ(processString("Hello World",
                          static_cast<int>(ConversionChoice::RemoveVowels)).get_c_str(),
            "Hll Wrld");
  EXPECT_STREQ(processString("Hello World",
                          static_cast<int>(ConversionChoice::RemoveSpaces)).get_c_str(),
            "HelloWorld");
  EXPECT_STREQ(processString("Hello World",
                          static_cast<int>(ConversionChoice::InvertWords)).get_c_str(),
            "olleH dlroW");
  EXPECT_STREQ(processString("Hello World",
                          static_cast<int>(ConversionChoice::SnakeCase)).get_c_str(),
            "hello_world");
  EXPECT_STREQ(processString("Hello World",
                          static_cast<int>(ConversionChoice::KebabCase)).get_c_str(),
            "hello-world");
  EXPECT_STREQ(
      processString("Test", static_cast<int>(ConversionChoice::LeetSpeak)).get_c_str(),
      "7357");
}

TEST(ProcessStringTest, InvalidChoice) {
  EXPECT_STREQ(processString("Hello", 99).get_c_str(), "hello"); // invalid choice falls back
}

//
// ======================================================
// 7. LOGGING TESTS (OPTIONAL)
// ======================================================
//

TEST(LoggingTest, ConversionLogging) {
  std::string input = "TeSt";
  ToggleCaseConversion conv;

  std::string result = conv.convert(input).get_c_str();
  logConversion("ToggleCase", input, result);

  EXPECT_STREQ(conv.convert(input).get_c_str(), "tEsT");
}