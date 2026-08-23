// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : SpellCheckerTestsDLL.cpp                            */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-01                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : DLL ABI Validation                                  */
/*                                                                   */
/* Description : Google Test suite validating exported DLL ABI       */
/*               behavior, lifecycle management, persistence,        */
/*               marshaling, and Nuspell integration.                */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-17  Nitish Singh    Initial implementation     */
/*********************************************************************/

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#ifdef __APPLE__
constexpr auto DICT_PATH =
    "/opt/homebrew/share/hunspell/en_US";
#else
constexpr auto DICT_PATH =
    "/usr/share/hunspell/en_US";
#endif

extern "C"
{
#include "LexisSpellCheckDLL.hpp"
}

// ============================================================
// TEST FIXTURE
// ============================================================

class SpellCheckerDLLTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        std::filesystem::remove(DICTIONARY_PATH);
    }
};

// ============================================================
// HANDLE LIFECYCLE
// ============================================================

TEST_F(SpellCheckerDLLTest, CreateSpellChecker)
{
    auto handle = createSpellChecker();

    ASSERT_NE(handle, nullptr);

    freeSpellChecker(handle);
}

TEST_F(SpellCheckerDLLTest, MultipleInstancesCreation)
{
    auto h1 = createSpellChecker();
    auto h2 = createSpellChecker();

    ASSERT_NE(h1, nullptr);
    ASSERT_NE(h2, nullptr);

    EXPECT_NE(h1, h2);

    freeSpellChecker(h1);
    freeSpellChecker(h2);
}

// ============================================================
// PERSONAL DICTIONARY INSERTION
// ============================================================

TEST_F(SpellCheckerDLLTest, InsertPersonalWord)
{
    auto handle = createSpellChecker();

    insertPersonalWord(handle, "apple");

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(handle,
                     "apple",
                     &suggestions),
        1);

    freeSpellChecker(handle);
}

TEST_F(SpellCheckerDLLTest, CaseNormalization)
{
    auto handle = createSpellChecker();

    insertPersonalWord(handle, "Data");

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(handle,
                     "DATA",
                     &suggestions),
        1);

    EXPECT_EQ(
        checkWordABI(handle,
                     "data",
                     &suggestions),
        1);

    freeSpellChecker(handle);
}

TEST_F(SpellCheckerDLLTest, DuplicateInsertion)
{
    auto handle = createSpellChecker();

    insertPersonalWord(handle, "apple");
    insertPersonalWord(handle, "apple");
    insertPersonalWord(handle, "APPLE");

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(handle,
                     "apple",
                     &suggestions),
        1);

    freeSpellChecker(handle);
}

// ============================================================
// PERSISTENCE
// ============================================================

TEST_F(SpellCheckerDLLTest, PersistenceAcrossInstances)
{
    {
        auto handle = createSpellChecker();

        insertPersonalWord(handle, "persistent");

        freeSpellChecker(handle);
    }

    auto handle = createSpellChecker();

    loadPersonalDictionary(handle);

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(handle,
                     "persistent",
                     &suggestions),
        1);

    freeSpellChecker(handle);
}

TEST_F(SpellCheckerDLLTest, MissingPersistenceFile)
{
    std::filesystem::remove(DICTIONARY_PATH);

    auto handle = createSpellChecker();

    EXPECT_NO_THROW(
        loadPersonalDictionary(handle));

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(handle,
                     "apple",
                     &suggestions),
        0);

    freeSpellChecker(handle);
}

// ============================================================
// DICTIONARY LOADING
// ============================================================

TEST_F(SpellCheckerDLLTest, InvalidDictionaryPath)
{
    auto handle = createSpellChecker();

    EXPECT_EQ(
        loadMainDictionary(
            handle,
            "/invalid/path"),
        0);

    freeSpellChecker(handle);
}

TEST_F(SpellCheckerDLLTest, ValidDictionaryPath)
{
    auto handle = createSpellChecker();

    if (!std::filesystem::exists(
            std::string(DICT_PATH) + ".aff") ||
        !std::filesystem::exists(
            std::string(DICT_PATH) + ".dic"))
    {
        GTEST_SKIP()
            << "Hunspell dictionary not installed";
    }

    EXPECT_EQ(
        loadMainDictionary(
            handle,
            DICT_PATH),
        1);

    freeSpellChecker(handle);
}

// ============================================================
// NUSPELL INTEGRATION
// ============================================================

TEST_F(SpellCheckerDLLTest, SuggestionsReturned)
{
    auto handle = createSpellChecker();

    if (!loadMainDictionary(
            handle,
            DICT_PATH))
    {
        GTEST_SKIP();
    }

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(handle,
                     "appple",
                     &suggestions),
        0);

    EXPECT_NE(suggestions, nullptr);

    freeSuggestionsBuffer(
        const_cast<char*>(suggestions));

    freeSpellChecker(handle);
}

TEST_F(SpellCheckerDLLTest, TrieHasPriorityOverDictionary)
{
    auto handle = createSpellChecker();

    insertPersonalWord(handle, "customword");

    if (!loadMainDictionary(
            handle,
            DICT_PATH))
    {
        GTEST_SKIP();
    }

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(handle,
                     "customword",
                     &suggestions),
        1);

    freeSpellChecker(handle);
}

// ============================================================
// ABI VALIDATION
// ============================================================

TEST_F(SpellCheckerDLLTest, NullHandleDictionaryLoad)
{
    EXPECT_EQ(
        loadMainDictionary(
            nullptr,
            DICT_PATH),
        0);
}

TEST_F(SpellCheckerDLLTest, NullHandleCheckWord)
{
    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(
            nullptr,
            "apple",
            &suggestions),
        -1);
}

TEST_F(SpellCheckerDLLTest, NullWordCheck)
{
    auto handle = createSpellChecker();

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(
            handle,
            nullptr,
            &suggestions),
        -1);

    freeSpellChecker(handle);
}

TEST_F(SpellCheckerDLLTest, NullSuggestionsPointer)
{
    auto handle = createSpellChecker();

    EXPECT_EQ(
        checkWordABI(
            handle,
            "apple",
            nullptr),
        -1);

    freeSpellChecker(handle);
}

// ============================================================
// SPECIAL CHARACTER VALIDATION
// ============================================================

TEST_F(SpellCheckerDLLTest, ZeroWidthSpaceInjection)
{
    auto handle = createSpellChecker();

    insertPersonalWord(handle, "apple");

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(
            handle,
            "ap\u200Bple",
            &suggestions),
        0);

    freeSpellChecker(handle);
}

TEST_F(SpellCheckerDLLTest, EmbeddedTabCharacter)
{
    auto handle = createSpellChecker();

    insertPersonalWord(handle, "apple");

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(
            handle,
            "app\tle",
            &suggestions),
        0);

    freeSpellChecker(handle);
}

TEST_F(SpellCheckerDLLTest, EmbeddedNewlineCharacter)
{
    auto handle = createSpellChecker();

    insertPersonalWord(handle, "apple");

    const char* suggestions = nullptr;

    EXPECT_EQ(
        checkWordABI(
            handle,
            "app\nle",
            &suggestions),
        0);

    freeSpellChecker(handle);
}

// ============================================================
// BUFFER MANAGEMENT
// ============================================================

TEST_F(SpellCheckerDLLTest, SuggestionsBufferLifecycle)
{
    auto handle = createSpellChecker();

    if (!loadMainDictionary(
            handle,
            DICT_PATH))
    {
        GTEST_SKIP();
    }

    const char* suggestions = nullptr;

    checkWordABI(
        handle,
        "appple",
        &suggestions);

    EXPECT_NO_THROW(
        freeSuggestionsBuffer(
            const_cast<char*>(suggestions)));

    freeSpellChecker(handle);
}