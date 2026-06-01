// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : LexisSpellChecker.hpp                               */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-05-20                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information           */
/*                                                                   */
/* Module      : Core/Text                                           */
/* Component   : Spell Check Engine                                  */
/* Thread Safe : No (Concurrent reads safe, writes mutating trie not) */
/* Complexity  : O(m) for lookup where m is word length              */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Provides hybrid spellchecking capabilities using a   */
/*               fast local Trie fallback alongside an advanced       */
/*               Nuspell dictionary backend.                         */
/*                                                                   */
/* Notes       : Relies on external .aff and .dic Nuspell assets.    */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-05-20  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef SPELLCHECKER_HPP
#define SPELLCHECKER_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include <nuspell/dictionary.hxx>

namespace Lexis::SpellCheck {

/**
 * @struct SpellResult
 * @brief Result structure returned by the Check() method.
 * 
 * Contains a boolean flag specifying correctness status and a list 
 * of potential correction strings if misspelled.
 */
struct SpellResult {
    bool isCorrect;
    std::vector<std::string> suggestions;
};

/**
 * @struct TrieNode
 * @brief Internal node structure for the fast local fallback dictionary.
 */
struct TrieNode {
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isEndOfWord = false;
};

/**
 * @class SpellChecker
 * @brief Core engine handling validation and auto-suggestion generation 
 *        for strings against loaded linguistic dictionaries.
 */
class SpellChecker {
public:
    /**
     * @brief Constructs a new SpellChecker object.
     */
    SpellChecker();

    /**
     * @brief Inserts a single word into the internal fallback Trie structure.
     * @param word The word string to inject.
     */
    void Insert(const std::string& word);

    /**
     * @brief Fast lookup to verify if a word exists in the local fallback Trie.
     * @param word The word string to search.
     * @return True if found, otherwise false.
     */
    bool Contains(const std::string& word) const;

    /**
     * @brief Loads a baseline minimal vocabulary sample dataset into the Trie.
     */
    void LoadSampleDictionary();

    /**
     * @brief Loads structural linguistic dictionary configuration packages.
     * @param path System file path directory pointing to the target assets.
     * @return True if successfully parsed and bound, false otherwise.
     */
    bool LoadDictionary(const std::string& path);

    /**
     * @brief Validates spelling of a given target token and generates suggestions.
     * @param word The incoming query token to analyze.
     * @return SpellResult structure mapping validation state and replacements.
     */
    SpellResult Check(const std::string& word) const;

    /**
     * @brief Automatically reads custom user dictionary structures from disk.
     */
    void LoadFromFile();

private:
    std::shared_ptr<TrieNode> root;               ///< Root entry point for internal lookup Trie
    std::unique_ptr<nuspell::Dictionary> m_dict;  ///< Dynamic wrapper over native Nuspell parser
    std::string m_dataPath = "dictionary.txt";    ///< Default system filename fallback context
};

} // namespace Lexis::SpellCheck

#endif // SPELLCHECKER_HPP