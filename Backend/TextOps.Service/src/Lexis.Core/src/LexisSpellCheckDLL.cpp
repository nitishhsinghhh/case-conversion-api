// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File         : LexisSpellCheckDLL.cpp                             */
/* Author       : Nitish Singh                                       */
/* Created      : 2026-05-25                                         */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module       : Core/Interop                                       */
/* Component    : Spell Check Engine Boundary                        */
/* Thread Safe  : No (Requires external locking if shared across     */
/*                threads)                                           */
/* Complexity   : Mutation/Search matches underlying engine          */
/*                complexities                                       */
/* API Status   : Stable                                             */
/* Exception Safety : Strong Guarantee (Catch-all standard guards)   */
/*********************************************************************/

#include "LexisSpellCheckDLL.hpp"
#include "SpellChecker.hpp"
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <algorithm>

/*********************************************************************/
/* Constants: Security Bounds Gate: 1 MB
/*********************************************************************/

namespace {
constexpr size_t MAX_WORD_LEN = 1024; 
}

/*********************************************************************/
/* Internal Helpers for Unmanaged String Lifecycles
/*********************************************************************/

static char* copyToMallocatedBuffer(const std::string& str) {
    char* output = static_cast<char*>(std::malloc(str.size() + 1));
    if (!output) {
        return nullptr;
    }
    std::memcpy(output, str.c_str(), str.size() + 1);
    return output;
}

/*********************************************************************/
/* Exported DLL Implementation (Extern "C")
/*********************************************************************/

extern "C" {

SPELL_API SpellCheckerHandle createSpellChecker() {
    try {
        auto instance = new Lexis::SpellCheck::SpellChecker();
        return reinterpret_cast<SpellCheckerHandle>(instance);
    } catch (...) {
        return nullptr;
    }
}

SPELL_API void freeSpellChecker(SpellCheckerHandle handle) {
    if (handle) {
        auto instance = reinterpret_cast<Lexis::SpellCheck::SpellChecker*>(handle);
        delete instance;
    }
}

SPELL_API int loadMainDictionary(SpellCheckerHandle handle, const char* path) {
    if (!handle || !path) {
        return 0;
    }
    try {
        auto instance = reinterpret_cast<Lexis::SpellCheck::SpellChecker*>(handle);
        return instance->LoadDictionary(path) ? 1 : 0;
    } catch (...) {
        return 0;
    }
}

SPELL_API void loadPersonalDictionary(SpellCheckerHandle handle) {
    if (!handle) return;
    try {
        auto instance = reinterpret_cast<Lexis::SpellCheck::SpellChecker*>(handle);
        instance->LoadFromFile();
    } catch (...) {}
}

SPELL_API void insertPersonalWord(SpellCheckerHandle handle, const char* word) {
    if (!handle || !word) return;
    if (std::strlen(word) > MAX_WORD_LEN) return;
    
    try {
        auto instance = reinterpret_cast<Lexis::SpellCheck::SpellChecker*>(handle);
        instance->Insert(std::string(word));
    } catch (...) {}
}

SPELL_API int checkWordABI(SpellCheckerHandle handle, const char* word, const char** outSuggestions) {
    if (!handle || !word || !outSuggestions) {
        return -1;
    }

    size_t word_len = std::strlen(word);
    if (word_len == 0 || word_len > MAX_WORD_LEN) {
        return -1;
    }

    try {
        auto instance = reinterpret_cast<Lexis::SpellCheck::SpellChecker*>(handle);
        std::string search_word(word);

        Lexis::SpellCheck::SpellResult result = instance->Check(search_word);

        if (result.isCorrect) {
            *outSuggestions = nullptr;
            return 1;
        }

        if (instance->Contains(search_word)) {
            *outSuggestions = nullptr;
            return 1;
        }

        if (result.suggestions.empty()) {
            *outSuggestions = nullptr;
        } else {
            std::stringstream ss;
            size_t max_items = std::min(result.suggestions.size(), static_cast<size_t>(5));
            
            for (size_t i = 0; i < max_items; ++i) {
                ss << result.suggestions[i];
                if (i + 1 < max_items) {
                    ss << "|";
                }
            }
            
            *outSuggestions = copyToMallocatedBuffer(ss.str());
            if (!*outSuggestions) {
                return -1; 
            }
        }

        return 0; 

    } catch (...) {
        return -1; 
    }
}

SPELL_API void freeSuggestionsBuffer(char* str) {
    if (str) {
        std::free(str);
    }
}

} // extern "C"