// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : LowerCaseConversion.cpp                             */
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
/* Description : Converts all alphabetic characters in the input     */
/*               string to lowercase using ASCII-based conversion.   */
/*                                                                   */
/* Notes       : - ASCII difference between upper and lower is 32    */
/*             : - Preserves non-alphabetic characters               */
/*             : - Performs in-place transformation on copy          */
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

#include "LowerCaseConversion.hpp"

ConversionResult LowerCaseConversion::convert(const std::string &input) const {
  std::string result = input;

  for (char &c : result) {
    if (c >= 'A' && c <= 'Z') {
      c = c + ('a' - 'A');
    }
  }

  return ConversionResult(result.c_str());
}