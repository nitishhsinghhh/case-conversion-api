// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ToggleCaseConversion.cpp                            */
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
/* Description : Toggles the case of alphabetic characters in the    */
/*               input string (lowercase to uppercase and vice       */
/*               versa).                                             */
/*                                                                   */
/* Notes       : - Reuses LowerCaseConversion and                    */
/*                 UpperCaseConversion                               */
/*             : - Non-alphabetic characters remain unchanged        */
/*             : - Character-wise transformation in linear time      */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/* 1.1        2026-04-12  Nitish Singh    Code quality improvements  */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "ToggleCaseConversion.hpp"
#include "LowerCaseConversion.hpp"
#include "UpperCaseConversion.hpp"

ConversionResult ToggleCaseConversion::convert(const std::string &input) const {
  LowerCaseConversion lowerConv;
  UpperCaseConversion upperConv;

  std::string result;

  for (char c : input) {
    if (c >= 'a' && c <= 'z') {
      std::string temp(1, c);
      result += ConversionResult(upperConv.convert(temp)).get_c_str();
    } else if (c >= 'A' && c <= 'Z') {
      std::string temp(1, c);
      result += ConversionResult(lowerConv.convert(temp)).get_c_str();
    } else {
      result += c; // Non-alphabetic characters unchanged
    }
  }

  return ConversionResult(result.c_str());
}