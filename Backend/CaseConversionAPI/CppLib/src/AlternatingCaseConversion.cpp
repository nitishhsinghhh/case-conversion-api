// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : AlternatingCaseConversion.cpp                       */
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
/*               starting with uppercase for each word.              */
/*                                                                   */
/* Notes       : - Reuses LowerCaseConversion and                    */
/*                 UpperCaseConversion implementations.              */
/*               - Alternation resets after whitespace characters.   */
/*               - Non-alphabetic characters are preserved.          */
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

#include "AlternatingCaseConversion.hpp"
#include "ConversionResult.hpp"
#include "LowerCaseConversion.hpp"
#include "UpperCaseConversion.hpp"

ConversionResult
AlternatingCaseConversion::convert(const std::string &input) const {
  LowerCaseConversion lowerConv;
  UpperCaseConversion upperConv;

  std::string finalResult;
  bool upper = true;

  for (char c : input) {
    if (std::isalpha(static_cast<unsigned char>(c))) {
      std::string temp(1, c);

      if (upper) {
        auto resObj = upperConv.convert(temp);
        finalResult += resObj.get_c_str();
      } else {
        auto resObj = lowerConv.convert(temp);
        finalResult += resObj.get_c_str();
      }

      upper = !upper;
    } else {
      finalResult += c;
      if (c == ' ') {
        upper = true;
      }
    }
  }
  return ConversionResult(finalResult.c_str());
}