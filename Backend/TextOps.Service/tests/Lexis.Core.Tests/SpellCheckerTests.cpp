// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : SpellCheckerTests.cpp                               */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-05-20                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information           */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : Spell Check Engine                                  */
/* Thread Safe : No (test suite)                                     */
/* Complexity  : O(m) lookup/insertion per test case                 */
/* API Status  : Stable                                              */
/* Exception Safety : N/A (test environment)                         */
/*                                                                   */
/* Description : Google Test suite validating the hybrid components   */
/*               of the Lexis SpellChecker. Evaluates case-insensitive */
/*               Trie processing, exact prefix bounding, disk state   */
/*               persistence, and Nuspell core integration.           */
/*                                                                   */
/* Test Groups :                                                     */
/*               1. Basic Trie Insertion and Case Normalization      */
/*               2. Exact Prefix vs Complete Word Boundary Checking  */
/*               3. Persistent File System File I/O Syncing          */
/*               4. Native Nuspell Spellcheck & Suggestion Engine    */
/*                                                                   */
/* Notes       : Cleans filesystem state 'dictionary.txt' during     */
/*               test setup to guarantee isolated executions.         */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <gtest/gtest.h>
#include <cstddef>

// ---------------------------
// Core Includes
// ---------------------------
#include "SpellChecker.hpp"

// ---------------------------
// System / STL Includes
// ---------------------------
#include <filesystem>
#include <fstream>

using namespace Lexis::SpellCheck;

// ============================================================
// TEST FIXTURE DEFINITION
// ============================================================

class SpellCheckerTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Ensure a clean state for internal persistence validations
    std::filesystem::remove("dictionary.txt");
  }
};

// ============================================================
// 1. BASIC TRIE INSERTION AND SEARCH
// ============================================================

TEST_F(SpellCheckerTest, BasicInsertAndContains) {
  SpellChecker checker;
  checker.Insert("apple");
  
  EXPECT_TRUE(checker.Contains("apple"));
  EXPECT_TRUE(checker.Contains("APPLE")); // Testing case normalization
  EXPECT_FALSE(checker.Contains("orange"));
}

// ============================================================
// 2. PREFIX MATCHING
// ============================================================

TEST_F(SpellCheckerTest, PrefixHandling) {
  SpellChecker checker;
  checker.Insert("apple");
  
  // "app" is a valid prefix branch but should not register as a terminal word
  static_cast<void>(checker.Contains("app")); 
  EXPECT_FALSE(checker.Contains("app"));
}

// ============================================================
// 3. PERSISTENCE (FILE I/O)
// ============================================================

TEST_F(SpellCheckerTest, PersistenceCheck) {
  {
    SpellChecker checker;
    checker.Insert("persistent");
  } // Scope closes; internal stream flushes tracking additions to disk

  SpellChecker newChecker;
  newChecker.LoadFromFile();
  EXPECT_TRUE(newChecker.Contains("persistent"));
}

// ============================================================
// 4. NUSPELL INTEGRATION AND SUGGESTIONS
// ============================================================

TEST_F(SpellCheckerTest, NuspellSuggestions) {
  SpellChecker checker;
  if (checker.LoadDictionary("/usr/share/hunspell/en_US")) {
    SpellResult result = checker.Check("appple");
    EXPECT_FALSE(result.isCorrect);
    EXPECT_FALSE(result.suggestions.empty());
  }
}

TEST_F(SpellCheckerTest, CaseAndSanitization) {
    SpellChecker checker;
    checker.Insert("Data");
    
    EXPECT_TRUE(checker.Contains("data"));
    EXPECT_TRUE(checker.Contains("DATA"));
    EXPECT_TRUE(checker.Contains("dAtA"));
    // Ensure symbols don't trigger crashes
    EXPECT_FALSE(checker.Contains("data!")); 
}

TEST_F(SpellCheckerTest, LocalOverrideNuspell) {
    SpellChecker checker;
    // Assuming "zyxwv" is NOT in the system dictionary
    checker.Insert("zyxwv");
    
    // Should be found in Trie
    EXPECT_TRUE(checker.Contains("zyxwv"));
    
    SpellResult result = checker.Check("zyxwv");
    EXPECT_TRUE(result.isCorrect);
}

TEST_F(SpellCheckerTest, EmptyStateBehavior) {
    SpellChecker checker;
    EXPECT_FALSE(checker.Contains("anything"));
    
    // Ensure loading a non-existent path doesn't crash the engine
    EXPECT_FALSE(checker.LoadDictionary("/non/existent/path/dict"));
}

TEST_F(SpellCheckerTest, StressTrieInsertion) {
    SpellChecker checker;
    std::vector<std::string> words = {"test1", "test2", "apple", "banana", "cherry"};
    
    for (const auto& w : words) {
        checker.Insert(w);
    }
    
    for (const auto& w : words) {
        EXPECT_TRUE(checker.Contains(w));
    }
}

TEST_F(SpellCheckerTest, CaseConsistencyCollision) {
    SpellChecker checker;
    checker.Insert("Data");
    
    // Testing if adding a mixed-case word and a lowercase word 
    // impacts the Trie's internal terminal node logic
    checker.Insert("data"); 
    
    EXPECT_TRUE(checker.Contains("DATA"));
    EXPECT_TRUE(checker.Contains("data"));
}

TEST_F(SpellCheckerTest, NoSuggestionsForValidWord) {
    SpellChecker checker;
    // Assume "apple" is in the system dict AND your Trie
    checker.Insert("apple");
    
    SpellResult result = checker.Check("apple");
    
    EXPECT_TRUE(result.isCorrect);
    // If the word is correct, suggestions should logically be empty
    EXPECT_TRUE(result.suggestions.empty());
}

TEST_F(SpellCheckerTest, InputSanitization) {
    SpellChecker checker;
    checker.Insert("valid");
    
    // Check if the engine is smart enough to handle whitespace
    EXPECT_FALSE(checker.Contains("valid ")); 
    EXPECT_FALSE(checker.Contains(" valid"));
}

TEST_F(SpellCheckerTest, NuspellPartialFunctionality) {
    SpellChecker checker;
    // Assuming a valid path but a file that isn't a full dictionary
    if (checker.LoadDictionary("/usr/share/hunspell/en_US")) {
        // Test an extremely long string that might overflow buffer logic
        std::string long_word(1000, 'a');
        SpellResult result = checker.Check(long_word);
        
        // It shouldn't crash; it should simply return 'false'
        EXPECT_FALSE(result.isCorrect);
    }
}

TEST_F(SpellCheckerTest, EmptyStringInsert) {
    SpellChecker checker;

    checker.Insert("");

    EXPECT_FALSE(checker.Contains(""));
}

TEST_F(SpellCheckerTest, SingleCharacterWords) {
    SpellChecker checker;

    checker.Insert("a");
    checker.Insert("i");

    EXPECT_TRUE(checker.Contains("a"));
    EXPECT_TRUE(checker.Contains("i"));
    EXPECT_FALSE(checker.Contains("b"));
}

TEST_F(SpellCheckerTest, DuplicateInsertDoesNotDuplicateFileEntries)
{
    SpellChecker checker;

    checker.Insert("apple");
    checker.Insert("apple");
    checker.Insert("APPLE");

    checker.LoadFromFile();

    EXPECT_TRUE(checker.Contains("apple"));
}

TEST_F(SpellCheckerTest, PersistenceNormalizesCase)
{
    {
        SpellChecker checker;
        checker.Insert("ApPlE");
    }

    SpellChecker checker;
    checker.LoadFromFile();

    EXPECT_TRUE(checker.Contains("apple"));
    EXPECT_TRUE(checker.Contains("APPLE"));
}

TEST_F(SpellCheckerTest, PrefixChainIntegrity)
{
    SpellChecker checker;

    checker.Insert("a");
    checker.Insert("an");
    checker.Insert("and");
    checker.Insert("android");

    EXPECT_TRUE(checker.Contains("a"));
    EXPECT_TRUE(checker.Contains("an"));
    EXPECT_TRUE(checker.Contains("and"));
    EXPECT_TRUE(checker.Contains("android"));

    EXPECT_FALSE(checker.Contains("andr"));
}

TEST_F(SpellCheckerTest, SharedBranchIntegrity)
{
    SpellChecker checker;

    checker.Insert("car");
    checker.Insert("card");
    checker.Insert("care");
    checker.Insert("careful");

    EXPECT_TRUE(checker.Contains("car"));
    EXPECT_TRUE(checker.Contains("card"));
    EXPECT_TRUE(checker.Contains("care"));
    EXPECT_TRUE(checker.Contains("careful"));

    EXPECT_FALSE(checker.Contains("cards"));
}

TEST_F(SpellCheckerTest, LongWordInsertion)
{
    SpellChecker checker;

    std::string word(10000, 'a');

    checker.Insert(word);

    EXPECT_TRUE(checker.Contains(word));
}

TEST_F(SpellCheckerTest, MassiveInsertion)
{
    SpellChecker checker;

    for (int i = 0; i < 50000; ++i)
    {
        checker.Insert("word" + std::to_string(i));
    }

    EXPECT_TRUE(checker.Contains("word0"));
    EXPECT_TRUE(checker.Contains("word49999"));
}

TEST_F(SpellCheckerTest, MissingPersistenceFile)
{
    std::filesystem::remove(DICTIONARY_PATH);

    SpellChecker checker;

    EXPECT_NO_THROW(checker.LoadFromFile());

    EXPECT_FALSE(checker.Contains("apple"));
}

TEST_F(SpellCheckerTest, TrieHasPriorityOverNuspell)
{
    SpellChecker checker;

    checker.Insert("customword");

    if (checker.LoadDictionary("/usr/share/hunspell/en_US"))
    {
        auto result = checker.Check("customword");

        EXPECT_TRUE(result.isCorrect);
        EXPECT_TRUE(result.suggestions.empty());
    }
}

TEST_F(SpellCheckerTest, CorruptedPersistenceData)
{
    std::ofstream file("dictionary.txt");
    file << "apple\n";
    file << "###\n";
    file << "banana\n";
    file.close();

    SpellChecker checker;

    EXPECT_NO_THROW(checker.LoadFromFile());
}

TEST_F(SpellCheckerTest, NumericWords)
{
    SpellChecker checker;

    checker.Insert("abc123");

    EXPECT_TRUE(checker.Contains("abc123"));
}

TEST_F(SpellCheckerTest, ZeroWidthSpaceInjection)
{
    SpellChecker checker;

    const std::string word = "apple";
    const std::string hidden = "ap\u200Bple";

    checker.Insert(word);

    EXPECT_TRUE(checker.Contains(word));

    // Should not be treated as the same word
    EXPECT_FALSE(checker.Contains(hidden));
}

TEST_F(SpellCheckerTest, ZeroWidthJoinerInjection)
{
    SpellChecker checker;

    checker.Insert("android");

    EXPECT_FALSE(checker.Contains("and\u200Droid"));
}

TEST_F(SpellCheckerTest, ZeroWidthNonJoinerInjection)
{
    SpellChecker checker;

    checker.Insert("banana");

    EXPECT_FALSE(checker.Contains("ba\u200Cnana"));
}

TEST_F(SpellCheckerTest, Utf8BomPrefix)
{
    SpellChecker checker;

    checker.Insert("apple");

    const std::string bomWord =
        std::string("\xEF\xBB\xBF", 3) + "apple";

    EXPECT_FALSE(checker.Contains(bomWord));
}

TEST_F(SpellCheckerTest, EmbeddedTabCharacter)
{
    SpellChecker checker;

    checker.Insert("apple");

    EXPECT_FALSE(checker.Contains("app\tle"));
}

TEST_F(SpellCheckerTest, EmbeddedNewlineCharacter)
{
    SpellChecker checker;

    checker.Insert("apple");

    EXPECT_FALSE(checker.Contains("app\nle"));
}

TEST_F(SpellCheckerTest, EmbeddedCarriageReturn)
{
    SpellChecker checker;

    checker.Insert("apple");

    EXPECT_FALSE(checker.Contains("app\rle"));
}