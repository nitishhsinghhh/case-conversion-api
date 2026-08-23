// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : Trie.hpp                                            */
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
/* Description : Header file providing a minimal, fast Trie-based    */
/*               dictionary structure for standalone lookups.        */
/*                                                                   */
/* Notes       : Lightweight version omitting heavy dynamic backends.*/
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-05-20  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef LEXIS_TRIE_HPP
#define LEXIS_TRIE_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace Lexis::SpellCheck {

/**
 * @struct TrieNode
 * @brief Internal node structure for the local lookup dictionary.
 */
struct TrieNode {
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isEndOfWord = false;
};

/**
 * @class SpellChecker
 * @brief Simple lookup manager handling text mapping and validation against 
 *        an internal character prefix-tree structure.
 */
class SpellChecker {
private:
    std::shared_ptr<TrieNode> root; 
public:
    /**
     * @brief Constructs a new SpellChecker object.
     */
    SpellChecker();

    /**
     * @brief Inserts a target character token directly into the Trie sequence.
     * @param word The literal target word string to append.
     */
    void Insert(const std::string& word);

    /**
     * @brief Queries character paths to locate matches within the internal tree structure.
     * @param word Target data payload query input.
     * @return True if word exists down to terminal flag, false otherwise.
     */
    bool Contains(const std::string& word) const;

    /**
     * @brief Generates boilerplate initial fallback structures in local memory.
     */
    void LoadSampleDictionary();
};

} // namespace Lexis::SpellCheck

#endif // LEXIS_TRIE_HPP