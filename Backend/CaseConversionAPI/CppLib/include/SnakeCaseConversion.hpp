// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : SnakeCaseConversion.hpp                             */
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
/* Description : Converts input strings into snake_case format       */
/*               where words are separated using underscores and     */
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

#ifndef SNAKECASECONVERSION_HPP
#define SNAKECASECONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <string>

/**
 * @class SnakeCaseConversion
 * @brief Concrete implementation of IStringConversion that converts to
 * snake_case.
 *
 * Example:
 * Input  : "Hello World Example"
 * Output : "hello_world_example"
 */
class SnakeCaseConversion : public IStringConversion {
public:
  /**
   * @brief Converts the input string to snake_case.
   * @param input The original string to convert.
   * @return A new string in snake_case format.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // SNAKECASECONVERSION_HPP