// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : LeetSpeakConversion.hpp                             */
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
/* Description : Converts input strings into leet speak (1337)       */
/*               format where selected alphabetic characters are     */
/*               replaced with numeric or symbolic equivalents.      */
/*                                                                   */
/* Notes       : - Implements the IStringConversion interface.       */
/*             : - Preserves unsupported characters as-is.           */
/*             : - Performs deterministic character substitution.    */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef LEETSPEAKCONVERSION_HPP
#define LEETSPEAKCONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <string>

/**
 * @class LeetSpeakConversion
 * @brief Concrete implementation of IStringConversion that converts strings
 * into leet speak.
 *
 * Example:
 * Input  : "Hello World"
 * Output : "H3ll0 W0rld"
 */
class LeetSpeakConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string into leet speak.
   * @param input The string to convert.
   * @return The leet speak version of the input.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // LEETSPEAKCONVERSION_HPP