// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : SentenceCaseConversion.cpp                          */
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
/* Description : Converts input string to sentence case by          */
/*               capitalizing the first character and converting     */
/*               remaining characters to lowercase.                  */
/*                                                                   */
/* Notes       : - Reuses LowerCaseConversion and UpperCaseConversion*/
/*             : - Safely handles empty input                        */
/*             : - Preserves non-alphabetic characters appropriately  */
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

#include "SentenceCaseConversion.hpp"
#include "LowerCaseConversion.hpp"
#include "UpperCaseConversion.hpp"

ConversionResult
SentenceCaseConversion::convert(const std::string &input) const {
  if (input.empty()) {
    return ConversionResult(input.c_str());
  }

  LowerCaseConversion lowerConv;
  UpperCaseConversion upperConv;

  std::string result = ConversionResult(lowerConv.convert(input)).get_c_str();
  std::string first(1, result[0]);
  result[0] = ConversionResult(upperConv.convert(first)).get_c_str()[0];

  return ConversionResult(result.c_str());
}