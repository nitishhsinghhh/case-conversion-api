// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : RemoveSpacesConversion.cpp                          */
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
/* Description : Removes all space characters from the input string. */
/*                                                                   */
/* Notes       : - Uses erase-remove idiom                           */
/*             : - Removes only space character ' '                  */
/*             : - Preserves all other whitespace characters         */
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

#include "RemoveSpacesConversion.hpp"
#include <algorithm>

ConversionResult
RemoveSpacesConversion::convert(const std::string &input) const {
  std::string result = input;
  result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
  return ConversionResult(result.c_str());
}