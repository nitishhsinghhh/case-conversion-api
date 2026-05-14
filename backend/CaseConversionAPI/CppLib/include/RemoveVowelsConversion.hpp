// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : RemoveVowelsConversion.hpp                          */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/String                                         */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Removes all vowel characters from the input string  */
/*               while preserving consonants, digits, and symbols.   */
/*                                                                   */
/* Notes       : - Implements the IStringConversion interface.       */
/*             : - Supports both uppercase and lowercase vowels.     */
/*             : - Preserves original character ordering.            */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef REMOVEVOWELSCONVERSION_HPP
#define REMOVEVOWELSCONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <string>

/**
 * @class RemoveVowelsConversion
 * @brief Concrete implementation of IStringConversion that removes vowels.
 *
 * Example:
 * Input  : "Hello World"
 * Output : "Hll Wrld"
 */
class RemoveVowelsConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string by removing all vowels.
   * @param input The original string to convert.
   * @return A new string with all vowels removed.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // REMOVEVOWELSCONVERSION_HPP