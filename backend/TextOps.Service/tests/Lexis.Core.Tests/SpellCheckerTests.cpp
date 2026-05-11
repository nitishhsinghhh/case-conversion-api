#include <gtest/gtest.h>
#include "spellcheck/SpellChecker.hpp"
#include <fstream>
#include <filesystem>

using namespace Lexis::SpellCheck;

class SpellCheckerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Optional: Start with a clean slate for persistence tests
        std::filesystem::remove("dictionary.txt");
    }
};

// Test 1: Basic Trie Insertion and Search
TEST_F(SpellCheckerTest, BasicInsertAndContains) {
    SpellChecker checker;
    checker.Insert("apple");
    
    EXPECT_TRUE(checker.Contains("apple"));
    EXPECT_TRUE(checker.Contains("APPLE")); // Testing normalization
    EXPECT_FALSE(checker.Contains("orange"));
}

// Test 2: Prefix Matching
TEST_F(SpellCheckerTest, PrefixHandling) {
    SpellChecker checker;
    checker.Insert("apple");
    
    // "app" is a prefix but not the full word
    EXPECT_FALSE(checker.Contains("app"));
}

// Test 3: Persistence (File I/O)
TEST_F(SpellCheckerTest, PersistenceCheck) {
    {
        SpellChecker checker;
        checker.Insert("persistent");
    } // checker goes out of scope and writes to file

    SpellChecker newChecker;
    newChecker.LoadFromFile();
    EXPECT_TRUE(newChecker.Contains("persistent"));
}

// Test 4: Nuspell Suggestions (if dictionary exists)
TEST_F(SpellCheckerTest, NuspellSuggestions) {
    SpellChecker checker;
    if (checker.LoadDictionary("/usr/share/hunspell/en_US")) {
        SpellResult result = checker.Check("appple");
        EXPECT_FALSE(result.isCorrect);
        EXPECT_FALSE(result.suggestions.empty());
    }
}