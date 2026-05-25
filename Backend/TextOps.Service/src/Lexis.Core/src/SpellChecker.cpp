// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : LexisSpellChecker.cpp                               */
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
/* Complexity  : O(m) for lookup/insert where m is word length       */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Implementation of a hybrid spellchecking architecture */
/*               utilizing an internal case-insensitive Trie structure*/
/*               integrated with a native Nuspell backup engine.     */
/*                                                                   */
/* Notes       : Relies on compile-time DICTIONARY_PATH definitions   */
/*               for automatic file persistence pipelines.            */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-05-20  Nitish Singh    Initial implementation     */
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
  std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
  
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
    
    // DICTIONARY_PATH comes from your CMake target_compile_definitions
    std::ofstream outfile(DICTIONARY_PATH, std::ios_base::app);
    if (outfile.is_open()) 
    {
      outfile << normalized << "\n";
    }
  }
}

bool SpellChecker::Contains(const std::string& word) const 
{
  std::string normalized = word;
  std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
  
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
  for (const auto& w : {"apple", "native"}) 
  {
    Insert(w); 
  }
}

bool SpellChecker::LoadDictionary(const std::string& p) 
{ 
  try 
  { 
    auto dict_obj = nuspell::Dictionary::load_from_path(p);
    m_dict = std::make_unique<nuspell::Dictionary>(std::move(dict_obj));
    return m_dict != nullptr; 
  } 
  catch (...) 
  { 
    return false; 
  } 
}

SpellResult SpellChecker::Check(const std::string& w) const 
{
  SpellResult r{false, {}};
  if (m_dict) 
  { 
    r.isCorrect = m_dict->spell(w); 
    if (!r.isCorrect) 
    {
      m_dict->suggest(w, r.suggestions); 
    }
  }
  return r;
}

} // namespace Lexis::SpellCheck