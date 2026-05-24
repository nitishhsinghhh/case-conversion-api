// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : CaseConverter.hpp                                   */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-05-20                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information           */
/*                                                                   */
/* Module      : Core/Text                                           */
/* Component   : Case Conversion                                     */
/* Thread Safe : No (Concurrent reads safe, writes mutating trie not) */
/* Complexity  : O(m) for lookup where m is word length              */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Provides a unified hybrid case conversion engine matching */
/*               an internal character Trie against external Nuspell */
/*               dictionary modules.                                 */
/*                                                                   */
/* Notes       : None.                                               */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-05-20  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef LEXIS_CASE_CONVERTER_HPP
#define LEXIS_CASE_CONVERTER_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <nuspell/dictionary.hxx>

namespace Lexis::SpellCheck {

/**
 * @struct SpellResult
 * @brief Evaluation metrics payload mapped from checking actions.
 */
struct SpellResult {
    bool isCorrect;
    std::vector<std::string> suggestions;
};

/**
 * @struct TrieNode
 * @brief Character element mapping step block for text prefix lookups.
 */
struct TrieNode {
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isEndOfWord = false;
};

/**
 * @class SpellChecker
 * @brief Orchestrates target string lookups down to isolated local Trie structures
 *        or managed Nuspell system dictionary instances.
 */
class SpellChecker {
public:
    /**
     * @brief Constructs a new SpellChecker object.
     */
    SpellChecker();

    /**
     * @brief Inserts an individual word directly into the backup character Trie.
     * @param word String token targeted for insertion.
     */
    void Insert(const std::string& word);

    /**
     * @brief Validates if a terminal matching word state matches within the local Trie.
     * @param word Source string payload context.
     * @return True if accurate entry is logged, false otherwise.
     */
    bool Contains(const std::string& word) const;

    /**
     * @brief Loads native standard sample words mockups down to the local Trie matrix.
     */
    void LoadSampleDictionary();

    /**
     * @brief Registers an external dict parsing sequence matching standard system pathing.
     * @param path Target workspace location storage boundary.
     * @return True if initialized correctly, false otherwise.
     */
    bool LoadDictionary(const std::string& path);

    /**
     * @brief Evaluates an abstract word payload token context against running structures.
     * @param word Raw target token reference string.
     * @return SpellResult mapped correction arrays matched to evaluation status.
     */
    SpellResult Check(const std::string& word) const;

private:
    std::shared_ptr<TrieNode> root;               ///< Local character node map pointer tracking block
    std::unique_ptr<nuspell::Dictionary> m_dict;  ///< Explicit platform pointer block managing Nuspell context
};

} // namespace Lexis::SpellCheck

#endif // LEXIS_CASE_CONVERTER_HPP