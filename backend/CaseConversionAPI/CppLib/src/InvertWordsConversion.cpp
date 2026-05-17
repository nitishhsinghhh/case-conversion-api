// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : InvertWordsConversion.cpp                           */
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
/* Description : Reverses characters of each word in the input       */
/*               string while preserving word order.                 */
/*                                                                   */
/* Notes       : - Uses string stream to split words                 */
/*             : - Uses std::reverse for in-place word reversal      */
/*             : - Preserves spacing between words                   */
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
#include "InvertWordsConversion.hpp"
#include <algorithm>
#include <sstream>

ConversionResult
InvertWordsConversion::convert(const std::string &input) const {
  std::istringstream iss(input);
  std::ostringstream oss;
  std::string word;
  bool first = true;

  while (iss >> word) {
    std::reverse(word.begin(), word.end());

    if (!first) {
      oss << ' ';
    }

    oss << word;
    first = false;
  }

  return ConversionResult(oss.str().c_str());
}