// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : CapitalizeWordsConversion.cpp                       */
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
/* Description : Converts input string so that each word starts      */
/*               with an uppercase letter followed by lowercase      */
/*               characters.                                         */
/*                                                                   */
/* Notes       : - Reuses LowerCaseConversion and                    */
/*               UpperCaseConversion implementations.                */
/*               - Processes input word-by-word using stream parsing */
/*               - Preserves single spacing between words.           */
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

#include "CapitalizeWordsConversion.hpp"
#include "LowerCaseConversion.hpp"
#include "UpperCaseConversion.hpp"
#include <sstream>

ConversionResult
CapitalizeWordsConversion::convert(const std::string &input) const {
  LowerCaseConversion lowerConv;
  UpperCaseConversion upperConv;

  std::istringstream iss(input);
  std::string word;
  std::string result;

  while (iss >> word) {
    if (!result.empty()) {
      result += " ";
    }

    // Convert whole word to lowercase first
    word = ConversionResult(lowerConv.convert(word))
               .get_c_str(); // Get C-string from ConversionResult

    // Capitalize first letter using UpperCaseConversion
    std::string firstChar(1, word[0]);
    firstChar = ConversionResult(upperConv.convert(firstChar)).get_c_str();
    word[0] = firstChar[0];

    result += word;
  }

  return ConversionResult(result.c_str());
}