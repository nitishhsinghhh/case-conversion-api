// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : RemoveSpacesConversion.hpp                          */
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
/* Description : Removes all whitespace characters from the input    */
/*               string while preserving original character order.   */
/*                                                                   */
/* Notes       : - Implements the IStringConversion interface.       */
/*             : - Performs linear-time filtering of whitespace.     */
/*             : - Returns a compacted string without delimiters.    */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef REMOVESPACECONVERSION_HPP
#define REMOVESPACECONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <string>

/**
 * @class RemoveSpacesConversion
 * @brief Concrete implementation of IStringConversion that removes spaces.
 *
 * Example:
 * Input  : "Hello World Example"
 * Output : "HelloWorldExample"
 */
class RemoveSpacesConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string by removing all spaces.
   * @param input The original string to convert.
   * @return A new string with all spaces removed.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // REMOVESPACECONVERSION_HPP