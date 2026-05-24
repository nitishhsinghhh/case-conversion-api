// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : KebabCaseConversion.cpp                             */
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
/* Description : Converts input string to kebab-case format by       */
/*               replacing spaces with hyphens and converting all    */
/*               characters to lowercase.                            */
/*                                                                   */
/* Notes       : - Reuses LowerCaseConversion implementation          */
/*             : - Converts spaces into hyphens                      */
/*             : - Preserves multiple consecutive separators          */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "KebabCaseConversion.hpp"
#include "LowerCaseConversion.hpp"

ConversionResult KebabCaseConversion::convert(const std::string &input) const {
  LowerCaseConversion lowerConv;

  std::string lower = ConversionResult(lowerConv.convert(input)).get_c_str();
  std::string result;

  for (char ch : lower) {
    if (ch == ' ') {
      result += '-';
    } else {
      result += ch;
    }
  }
  return ConversionResult(result.c_str());
}