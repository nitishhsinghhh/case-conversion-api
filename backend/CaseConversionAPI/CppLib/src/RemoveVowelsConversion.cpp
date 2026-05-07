// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : RemoveVowelsConversion.cpp                          */
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
/* Description : Removes all vowel characters from the input string. */
/*                                                                   */
/* Notes       : - Reuses LowerCaseConversion for normalization      */
/*             : - Removes both uppercase and lowercase vowels       */
/*             : - Preserves non-vowel characters in original form   */
/*             : - Maintains positional iteration consistency        */
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

#include "RemoveVowelsConversion.hpp"
#include "LowerCaseConversion.hpp"

ConversionResult
RemoveVowelsConversion::convert(const std::string &input) const {
  LowerCaseConversion lowerConv;
  std::string lower = ConversionResult(lowerConv.convert(input)).get_c_str();
  std::string result;
  for (size_t i = 0; i < input.size(); ++i) {
    char ch = lower[i];
    if (ch != 'a' && ch != 'e' && ch != 'i' && ch != 'o' && ch != 'u') {
      result += input[i];
    }
  }

  return ConversionResult(result.c_str());
}