// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ConversionTypeEnum.hpp                              */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Types                                          */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(1)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : No-Throw Guarantee                             */
/*                                                                   */
/* Description : Enumerates supported string conversion strategies   */
/*               exposed through the native DLL interface and        */
/*               internal conversion engine.                         */
/*                                                                   */
/* Notes       : - Values map directly to integer conversion IDs.    */
/*             : - Used by processStringDLL for strategy selection.  */
/*             : - Stable numeric ordering maintained for ABI        */
/*             :   compatibility across releases.                    */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial enum definition    */
/*********************************************************************/

#ifndef CONVERSIONTYPEENUM_HPP
#define CONVERSIONTYPEENUM_HPP

/**
 * @enum ConversionChoice
 * @brief Enumerates supported string conversion strategies.
 *
 * Each enum value corresponds to a conversion type that can be
 * selected by passing its integer value to processStringDLL.
 */
enum class ConversionChoice : int {
  Alternating = 1,  ///< Alternating Case
  Capitalize = 2,   ///< Capitalize Words
  Lower = 3,        ///< Lower Case
  Upper = 4,        ///< Upper Case
  Sentence = 5,     ///< Sentence Case
  Toggle = 6,       ///< Toggle Case
  Reverse = 7,      ///< Reverse String
  RemoveVowels = 8, ///< Remove Vowels
  RemoveSpaces = 9, ///< Remove Spaces
  InvertWords = 10, ///< Invert Words
  SnakeCase = 11,   ///< Snake Case
  KebabCase = 12,   ///< Kebab Case
  LeetSpeak = 13    ///< Leet Speak
};

#endif // CONVERSIONTYPEENUM_HPP