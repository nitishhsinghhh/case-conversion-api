// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : CapitalizeWordsConversion.hpp                       */
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
/* Description : Converts input string by capitalizing the first     */
/*               letter of each word while converting remaining      */
/*               characters to lowercase.                            */
/*                                                                   */
/* Notes       : Implements the IStringConversion interface.         */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef CAPITALIZEWORDSCONVERSION_HPP
#define CAPITALIZEWORDSCONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"

/**
 * @class CapitalizeWordsConversion
 * @brief Concrete implementation of IStringConversion that capitalizes
 *        the first letter of each word in a string.
 *
 * Example:
 * Input  : "hello world"
 * Output : "Hello World"
 */
class CapitalizeWordsConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string by capitalizing the first letter of each
   * word.
   * @param input The original string to convert.
   * @return A new string with each word capitalized.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // CAPITALIZEWORDSCONVERSION_HPP