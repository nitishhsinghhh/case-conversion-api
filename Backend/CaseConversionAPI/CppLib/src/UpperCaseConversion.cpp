// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : UpperCaseConversion.cpp                             */
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
/* Description : Converts all characters in the input string to      */
/*               uppercase format using ASCII conversion.            */
/*                                                                   */
/* Notes       : - ASCII difference between upper and lower is 32    */
/*             : - Preserves non-alphabetic characters               */
/*             : - Character-wise transformation in linear time      */
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

#include "UpperCaseConversion.hpp"

ConversionResult UpperCaseConversion::convert(const std::string &input) const {
  std::string result = input;

  for (char &c : result) {
    if (c >= 'a' && c <= 'z') {
      c = c - ('a' - 'A');
    }
  }

  return ConversionResult(result.c_str());
}