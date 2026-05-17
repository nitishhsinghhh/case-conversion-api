// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ConversionResult.cpp                                */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-30                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Memory                                         */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Strong Guarantee (where applicable)            */
/*                                                                   */
/* Description : Implements RAII-based ownership model for           */
/*               dynamically allocated C-style strings.              */
/*               Provides deep copy and move semantics for safe      */
/*               transfer across ABI boundaries.                     */
/*                                                                   */
/* Notes       : - Ensures deterministic memory cleanup via delete[]  */
/*             : - Designed for interoperability with managed runtimes*/
/*             : - Null-safe handling across all operations           */
/*             : - Implements Rule of Five                           */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-30  Nitish Singh    Initial implementation     */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "ConversionResult.hpp"

/*********************************************************************/
/* Constructor                                                       */
/*********************************************************************/

ConversionResult::ConversionResult(const char *input) {
  if (input) {
    data = new char[std::strlen(input) + 1];
    size_t length = std::strlen(input);
    std::memcpy(data, input, length + 1);
  } else {
    data = nullptr;
  }
}

/*********************************************************************/
/* Destructor                                                        */
/*********************************************************************/

ConversionResult::~ConversionResult() { delete[] data; }

/*********************************************************************/
/* Copy Constructor                                                  */
/*********************************************************************/

ConversionResult::ConversionResult(const ConversionResult &other) {
  if (other.data) {
    size_t length = std::strlen(other.data);
    data = new char[length + 1];
    std::memcpy(data, other.data, length + 1);
  } else {
    data = nullptr;
  }
}

/*********************************************************************/
/* Copy Assignment                                                   */
/*********************************************************************/

ConversionResult &ConversionResult::operator=(const ConversionResult &other) {
  if (this != &other) {
    char *new_data = nullptr;
    if (other.data) {
      size_t length = std::strlen(other.data);
      new_data = new char[length + 1];
      std::memcpy(new_data, other.data, length + 1);
    }

    // Clean up old data and assign new
    delete[] data;
    data = new_data;
  }
  return *this;
}

/*********************************************************************/
/* Move Constructor                                                  */
/*********************************************************************/

ConversionResult::ConversionResult(ConversionResult &&other) noexcept
    : data(other.data) {
  other.data = nullptr;
}

/*********************************************************************/
/* Move Assignment                                                   */
/*********************************************************************/

ConversionResult &
ConversionResult::operator=(ConversionResult &&other) noexcept {
  if (this != &other) {
    delete[] data;
    data = other.data;
    other.data = nullptr;
  }
  return *this;
}

/*********************************************************************/
/* Accessor                                                          */
/*********************************************************************/

const char *ConversionResult::get_c_str() const { return data; }