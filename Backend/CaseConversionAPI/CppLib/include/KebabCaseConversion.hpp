// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : KebabCaseConversion.hpp                             */
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
/* Description : Converts input strings into kebab-case format       */
/*               where words are separated using hyphens and all     */
/*               characters are normalized to lowercase.             */
/*                                                                   */
/* Notes       : - Implements the IStringConversion interface.       */
/*             : - Handles consecutive whitespace safely.            */
/*             : - Prevents trailing delimiter generation.           */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef KEBABCASECONVERSION_HPP
#define KEBABCASECONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <string>

/**
 * @class KebabCaseConversion
 * @brief Concrete implementation of IStringConversion that converts strings to
 * kebab-case.
 *
 * Example:
 * Input  : "Hello World Example"
 * Output : "hello-world-example"
 */
class KebabCaseConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string to kebab-case.
   * @param input The string to convert.
   * @return The kebab-case version of the input.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // KEBABCASECONVERSION_HPP