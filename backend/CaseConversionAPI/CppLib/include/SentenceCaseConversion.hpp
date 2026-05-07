// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : SentenceCaseConversion.hpp                          */
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
/* Description : Converts input strings into sentence case where     */
/*               the first alphabetic character is capitalized and   */
/*               remaining characters are normalized to lowercase.   */
/*                                                                   */
/* Notes       : - Implements the IStringConversion interface.       */
/*             : - Preserves non-alphabetic characters safely.       */
/*             : - Performs deterministic ASCII normalization.       */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef SENTENCECASECONVERSION_HPP
#define SENTENCECASECONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <string>

/**
 * @class SentenceCaseConversion
 * @brief Concrete implementation of IStringConversion that converts to sentence
 * case.
 *
 * Example:
 * Input  : "hELLO WORLD"
 * Output : "Hello world"
 */
class SentenceCaseConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string to sentence case.
   * @param input The string to convert.
   * @return The converted string with first letter capitalized.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // SENTENCECASECONVERSION_HPP