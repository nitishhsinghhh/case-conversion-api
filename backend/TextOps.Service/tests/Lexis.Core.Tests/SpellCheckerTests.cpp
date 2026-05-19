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
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
*********************************************************************/

#include <gtest/gtest.h>

// ---------------------------
// Core Includes
// ---------------------------
#include "spellcheck/SpellChecker.hpp"

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