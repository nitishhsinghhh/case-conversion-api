// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : UpperCaseConversion.hpp                             */
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
/* Description : Converts all alphabetic characters in the input     */
/*               string to uppercase while preserving                */
/*               non-alphabetic characters unchanged.                */
/*                                                                   */
/* Notes       : - Implements the IStringConversion interface.       */
/*             : - Uses deterministic ASCII transformation rules.    */
/*             : - Preserves whitespace, digits, and symbols.        */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef UPPERCASECONVERSION_HPP
#define UPPERCASECONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <string>

/**
 * @class UpperCaseConversion
 * @brief Converts a string to uppercase.
 *
 * Example:
 * Input  : "Hello World!"
 * Output : "HELLO WORLD!"
 */
class UpperCaseConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string to uppercase.
   * @param input The string to convert.
   * @return Converted string in uppercase.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // UPPERCASECONVERSION_HPP