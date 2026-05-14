// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ReverseConversion.hpp                               */
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
/* Description : Reverses the character sequence of the input        */
/*               string while preserving all original characters.    */
/*                                                                   */
/* Notes       : - Implements the IStringConversion interface.       */
/*             : - Performs in linear time complexity.               */
/*             : - Preserves whitespace and symbol placement         */
/*             :   relative to reversal order.                       */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef REVERSECONVERSION_HPP
#define REVERSECONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <string>

/**
 * @class ReverseConversion
 * @brief Concrete implementation of IStringConversion that reverses a string.
 *
 * Example:
 * Input  : "Hello"
 * Output : "olleH"
 */
class ReverseConversion : public IStringConversion {
public:
  /**
   * @brief Reverses the input string.
   * @param input The string to be reversed.
   * @return A new string with characters in reverse order.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // REVERSECONVERSION_HPP