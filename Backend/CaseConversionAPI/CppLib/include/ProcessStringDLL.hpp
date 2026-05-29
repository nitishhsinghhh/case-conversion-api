// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ProcessStringDLL.hpp                                */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Interop/Native                                      */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Declares exported native DLL interface used for     */
/*               unmanaged and C# P/Invoke-based string conversion.  */
/*                                                                   */
/* Notes       : - Provides C-compatible ABI interface.              */
/*             : - Supports cross-platform symbol visibility.        */
/*             : - Caller is responsible for freeing allocated       */
/*             :   memory using freeString().                        */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef PROCESSSTRINGDLL_HPP
#define PROCESSSTRINGDLL_HPP

/*********************************************************************/
/* Platform-Specific API Macros                                      */
/*                                                                   */
/*  Configures symbol visibility for the dynamic linker.             */
/* - Windows: Uses __declspec to manage DLL export/import tables.    */
/* - macOS/Linux: Uses visibility attributes to ensure P/Invoke      */
/*   can locate native entry points.                                 */
/*********************************************************************/

#ifdef _WIN32
#ifdef PROCESSSTRING_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
// CRITICAL FOR MACOS: Force symbol visibility
#define API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Converts input string based on choice and returns a newly allocated
 * C-string.
 *
 * @param input  C-string input.
 * @param len    Length of the input string.
 * @param choice Integer conversion choice.
 * @return C-string result (valid until next call). Caller must free using
 * freeString.
 */
API const char *processStringDLL(const char *input, int len, int choice,
                                 const char *traceId);

/**
 * @brief Frees memory allocated by processStringDLL
 *
 * Important: C# must call this to avoid memory leaks since DLL allocates
 * memory on the heap.
 *
 * This is a common pattern for C-style interop where the callee allocates
 * memory and the caller is responsible for freeing it.
 *
 */
API void freeString(char *str);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // PROCESSSTRINGDLL_HPP