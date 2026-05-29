// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ConversionResult.hpp                                */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-30                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Memory                                         */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : No                                                   */
/* Complexity  : O(n) Copy / O(1) Move                               */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : RAII wrapper for managing string conversion results */
/*               across native ABI boundaries using deterministic    */
/*               ownership semantics and Rule of 5 compliance.       */
/*                                                                   */
/* Notes       : - Implements deep copy semantics for memory safety. */
/*             : - Supports move semantics for zero-copy transfers.  */
/*             : - Designed for interoperability with C-style ABI    */
/*             :   ownership contracts (malloc/free model).          */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-30  Nitish Singh    Initial Rule of 5 support  */
/*********************************************************************/

#ifndef CONVERSION_RESULT_HPP
#define CONVERSION_RESULT_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstring>
#include <utility>

/**
 * @class ConversionResult
 * @brief RAII wrapper for string conversion results.
 *
 * Encapsulates a dynamically allocated C-style string and ensures
 * deterministic memory management across ABI boundaries.
 *
 * Example:
 * ConversionResult result("HELLO");
 * const char* output = result.get_c_str(); // Safe access
 */
class ConversionResult {
private:
  /// Pointer to heap-allocated C-style string
  char *data;

public:
  /**
   * @brief Constructs a ConversionResult from input string.
   * @param input Null-terminated C-string to copy.
   */
  explicit ConversionResult(const char *input);

  /**
   * @brief Destructor releases allocated memory.
   */
  ~ConversionResult();

  /*****************************************************************/
  /* Rule of 5: Copy Semantics                                     */
  /*****************************************************************/

  /**
   * @brief Copy constructor (deep copy).
   * @param other Source object to copy from.
   */
  ConversionResult(const ConversionResult &other);

  /**
   * @brief Copy assignment operator (deep copy).
   * @param other Source object to assign from.
   * @return Reference to current object.
   */
  ConversionResult &operator=(const ConversionResult &other);

  /*****************************************************************/
  /* Rule of 5: Move Semantics (Performance Optimization)           */
  /*****************************************************************/

  /**
   * @brief Move constructor (transfers ownership).
   * @param other Source object to move from.
   */
  ConversionResult(ConversionResult &&other) noexcept;

  /**
   * @brief Move assignment operator (transfers ownership).
   * @param other Source object to move from.
   * @return Reference to current object.
   */
  ConversionResult &operator=(ConversionResult &&other) noexcept;

  /**
   * @brief Returns the underlying C-style string.
   * @return Pointer to null-terminated string.
   */
  [[nodiscard]] const char *get_c_str() const;
};

#endif // CONVERSION_RESULT_HPP