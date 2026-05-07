// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : InvertWordsConversion.hpp                           */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
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
/* Description : Converts input string by reversing each word        */
/*               individually while preserving original word order.  */
/*                                                                   */
/* Notes       : Implements the IStringConversion interface.         */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef INVERTWORDSCONVERSION_HPP
#define INVERTWORDSCONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <string>

/**
 * @class InvertWordsConversion
 * @brief Concrete implementation of IStringConversion that inverts each word.
 *
 * Example:
 * Input  : "Hello World"
 * Output : "olleH dlroW"
 */
class InvertWordsConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string by inverting each word.
   * @param input The original string to convert.
   * @return A new string with each word reversed.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // INVERTWORDSCONVERSION_HPP