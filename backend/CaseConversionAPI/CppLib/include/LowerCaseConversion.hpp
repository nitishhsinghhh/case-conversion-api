// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : LowerCaseConversion.hpp                             */
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
/* Description : Converts all alphabetic characters in a string to   */
/*               lowercase while preserving non-alphabetic           */
/*               characters unchanged.                               */
/*                                                                   */
/* Notes       : - Implements the IStringConversion interface.       */
/*             : - Uses standard character transformation utilities. */
/*             : - Performs locale-independent ASCII conversion.     */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef LOWERCASECONVERSION_HPP
#define LOWERCASECONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <algorithm>
#include <cctype>

/**
 * @class LowerCaseConversion
 * @brief Concrete implementation of IStringConversion that converts strings to
 * lowercase.
 *
 * Example:
 * Input  : "HELLO WORLD"
 * Output : "hello world"
 */
class LowerCaseConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string to lowercase.
   * @param input The string to convert.
   * @return A new string where all alphabetic characters are lowercase.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // LOWERCASECONVERSION_HPP