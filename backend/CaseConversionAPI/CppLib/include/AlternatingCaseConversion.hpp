// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : AlternatingCaseConversion.hpp                       */
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
/* Description : Converts input string into alternating case format  */
/*               using uppercase/lowercase alternation beginning     */
/*               with an uppercase character.                        */
/*                                                                   */
/* Notes       : Implements the IStringConversion interface.         */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef ALTERNATINGCASECONVERSION_HPP
#define ALTERNATINGCASECONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"

/**
 * @class AlternatingCaseConversion
 * @brief Concrete implementation of IStringConversion that alternates
 *        the case of each character in a string.
 *
 * Example:
 * Input  : "hello world"
 * Output : "HeLlO WoRlD"
 */
class AlternatingCaseConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string by alternating the case of each character.
   * @param input The original string to convert.
   * @return A new string with alternating character case.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // ALTERNATINGCASECONVERSION_HPP