// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : LexisSpellChecker.cpp                               */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-05-20                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Text                                           */
/* Component   : Spell Check Engine                                  */
/* Thread Safe : No (Concurrent reads safe, writes mutating trie not)*/
/* Complexity  : O(m) for lookup/insert where m is word length       */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Implementation of a hybrid spellchecking arch       */
/*               utilizing an internal case-insensitive Trie         */
/*               structure integrated with a native Nuspell backup   */
/*               engine.                                             */
/*                                                                   */
/* Notes       : Relies on compile-time DICTIONARY_PATH definitions  */
/*               for automatic file persistence pipelines.           */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-05-20  Nitish Singh    Initial implementation     */
/* 1.1        2026-06-01   Nitish Singh   Added Hybrid Trie-Nuspell  */
/*                                        priority logic.            */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "SpellChecker.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>

namespace Lexis::SpellCheck {

SpellChecker::SpellChecker() 
  : root(std::make_shared<TrieNode>())
  , m_dict(nullptr) 
{
}

void SpellChecker::Insert(const std::string& word)
{
    std::string normalized = word;

    std::transform(
        normalized.begin(),
        normalized.end(),
        normalized.begin(),
        ::tolower);

    auto curr = root;

    for (char ch : normalized)
    {
        if (curr->children.find(ch) == curr->children.end())
        {
            curr->children[ch] = std::make_shared<TrieNode>();
        }

        curr = curr->children[ch];
    }

    if (!curr->isEndOfWord)
    {
        curr->isEndOfWord = true;

        std::ofstream outfile(DICTIONARY_PATH, std::ios_base::app);

        if (outfile.is_open())
        {
            outfile << normalized << '\n';
        }
    }
}

bool SpellChecker::Contains(const std::string& word) const
{
    if (word.empty())
    {
        return false;
    }

    std::string normalized = word;

    std::transform(
        normalized.begin(),
        normalized.end(),
        normalized.begin(),
        ::tolower);

    auto curr = root;

    for (char ch : normalized)
    {
        if (curr->children.find(ch) == curr->children.end())
        {
            return false;
        }

        curr = curr->children[ch];
    }

    return curr->isEndOfWord;
}

void SpellChecker::LoadFromFile()
{
    std::ifstream infile(DICTIONARY_PATH);

    std::string word;

    while (infile >> word)
    {
        auto curr = root;

        for (char ch : word)
        {
            if (curr->children.find(ch) == curr->children.end())
            {
                curr->children[ch] = std::make_shared<TrieNode>();
            }

            curr = curr->children[ch];
        }

        curr->isEndOfWord = true;
    }
}

void SpellChecker::LoadSampleDictionary()
{
    for (const auto& word : {"apple", "native"})
    {
        Insert(word);
    }
}

bool SpellChecker::LoadDictionary(const std::string& path)
{
    try
    {
        auto dictObj = nuspell::Dictionary::load_from_path(path);

        m_dict =
            std::make_unique<nuspell::Dictionary>(std::move(dictObj));

        return m_dict != nullptr;
    }
    catch (...)
    {
        return false;
    }
}

SpellResult SpellChecker::Check(const std::string& word) const
{
    SpellResult result{false, {}};

    // Fast path: local Trie lookup
    if (Contains(word))
    {
        result.isCorrect = true;
        return result;
    }

    // Fallback: Nuspell dictionary
    if (m_dict)
    {
        result.isCorrect = m_dict->spell(word);

        if (!result.isCorrect)
        {
            m_dict->suggest(word, result.suggestions);
        }
    }

    return result;
}

} // namespace Lexis::SpellCheck