// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File         : LexisSpellCheckDLL.hpp                             */
/* Author       : Nitish Singh                                       */
/* Created      : 2026-05-25                                         */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module       : Interop/Native                                     */
/* Component    : Spell Check Engine Boundary                        */
/* Thread Safe  : No (Trie and Nuspell configurations are instance   */
/*                bound)                                             */
/* Complexity   : O(1) for layer dispatch                            */
/* API Status   : Stable                                             */
/* Exception Safety : Strong Guarantee                               */
/*                                                                   */
/* Description  : Declares exported native DLL interface used for    */
/* unmanaged and C# P/Invoke-based interactive spell                 */
/* checking, lifecycle validation, and trie mutations.               */
/*********************************************************************/

#ifndef LEXIS_SPELLCHECK_DLL_HPP
#define LEXIS_SPELLCHECK_DLL_HPP

#ifdef _WIN32
    #ifdef LEXISSPELLCHECK_EXPORTS
        #define SPELL_API __declspec(dllexport)
    #else
        #define SPELL_API __declspec(dllimport)
    #endif
#else
    #define SPELL_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque handle representing an active instance of the SpellChecker.
 * Prevents exposing internal class layouts over the ABI boundary.
 */
typedef struct SpellCheckerOpaque* SpellCheckerHandle;

/**
 * @brief Allocation Lifecycle: Instantiates a native SpellChecker execution context.
 * @return Handle pointing to the allocated engine, or NULL on allocation error.
 */
SPELL_API SpellCheckerHandle createSpellChecker();

/**
 * @brief Destruction Lifecycle: Disposes of the execution context and frees memory.
 * @param handle The active context engine pointer to dispose.
 */
SPELL_API void freeSpellChecker(SpellCheckerHandle handle);

/**
 * @brief Orchestrates Nuspell dictionary installation mapping.
 * @param handle Active context handle.
 * @param path Unmanaged path to dictionary descriptor file (without extension).
 * @return 1 on successful initialization, 0 if data layout lookup fails.
 */
SPELL_API int loadMainDictionary(SpellCheckerHandle handle, const char* path);

/**
 * @brief Populates the internal trie using the persistent text data store.
 * @param handle Active context handle.
 */
SPELL_API void loadPersonalDictionary(SpellCheckerHandle handle);

/**
 * @brief Mutates the personal trie data store by inserting a verified token phrase.
 * @param handle Active context handle.
 * @param word Content string to save into the persistent workspace layer.
 */
SPELL_API void insertPersonalWord(SpellCheckerHandle handle, const char* word);

/**
 * @brief Evaluates an unmanaged text buffer against the combined lookup engine tiers.
 * @param handle Active context handle.
 * @param word Raw target unmanaged string buffer.
 * @param outSuggestions Reference pointer to receive unmanaged character array address.
 * Suggestions are serialized using '|' character delimiter boundaries.
 * @return Value matrix tracking match states:
 * 1  -> Token is validated as clean (Matches Nuspell rules or Personal Trie).
 * 0  -> Misspelled word. Suggestions string written to outSuggestions destination.
 * -1  -> Parametric verification or exception state fault occurred.
 */
SPELL_API int checkWordABI(SpellCheckerHandle handle, const char* word, const char** outSuggestions);

/**
 * @brief Frees unmanaged serialization memory strings generated via checkWordABI paths.
 * @param str Memory target address generated via unmanaged malloc pipeline.
 */
SPELL_API void freeSuggestionsBuffer(char* str);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LEXIS_SPELLCHECK_DLL_HPP