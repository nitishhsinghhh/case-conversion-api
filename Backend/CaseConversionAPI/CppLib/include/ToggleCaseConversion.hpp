// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ToggleCaseConversion.hpp                            */
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
/* Description : Converts input strings by toggling the case of      */
/*               each alphabetic character while preserving          */
/*               non-alphabetic characters unchanged.                */
/*                                                                   */
/* Notes       : - Implements the IStringConversion interface.       */
/*             : - Lowercase characters become uppercase.            */
/*             : - Uppercase characters become lowercase.            */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef TOGGLECASECONVERSION_HPP
#define TOGGLECASECONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <string>

/**
 * @class ToggleCaseConversion
 * @brief Converts a string by toggling character case.
 *
 * Example:
 * Input  : "HeLLo"
 * Output : "hEllO"
 */
class ToggleCaseConversion : public IStringConversion {
public:
  /**
   * @brief Toggles the case of each character in the input string.
   * @param input The string to convert.
   * @return Converted string with toggled case.
   */
  ConversionResult convert(const std::string &input) const override;
};

#endif // TOGGLECASECONVERSION_HPP