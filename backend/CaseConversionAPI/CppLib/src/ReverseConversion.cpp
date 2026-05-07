// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ReverseConversion.cpp                               */
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
/* Description : Reverses the entire input string.                   */
/*                                                                   */
/* Notes       : - Uses std::reverse for in-place reversal           */
/*             : - Preserves all characters including whitespace     */
/*             : - Produces full string reversal without segmentation*/
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

#include "ReverseConversion.hpp"
#include <algorithm>

ConversionResult ReverseConversion::convert(const std::string &input) const {
  std::string result = input;
  std::reverse(result.begin(), result.end());
  return ConversionResult(result.c_str());
}