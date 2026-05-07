// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : LeetSpeakConversion.cpp                             */
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
/* Description : Converts input string into leetspeak format by      */
/*               replacing selected alphabetic characters with       */
/*               numeric representations.                            */
/*                                                                   */
/* Notes       : - Uses unordered_map for character mapping          */
/*             : - Preserves characters not present in mapping       */
/*             : - Supports both uppercase and lowercase inputs      */
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

#include "LeetSpeakConversion.hpp"
#include <unordered_map>

ConversionResult LeetSpeakConversion::convert(const std::string &input) const {
  std::unordered_map<char, std::string> leetMap = {
      {'a', "4"}, {'A', "4"}, {'e', "3"}, {'E', "3"}, {'i', "1"}, {'I', "1"},
      {'o', "0"}, {'O', "0"}, {'s', "5"}, {'S', "5"}, {'t', "7"}, {'T', "7"}};

  std::string result;

  for (char ch : input) {
    if (leetMap.find(ch) != leetMap.end()) {
      result += leetMap[ch];
    } else {
      result += ch;
    }
  }

  return ConversionResult(result.c_str());
}