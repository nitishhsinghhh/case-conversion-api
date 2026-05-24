// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : TestHelpers.hpp                                     */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Tests/Utilities                                     */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Provides reusable helper utilities for logging and  */
/*               displaying string conversion test results.          */
/*                                                                   */
/* Notes       : - Contains inline utility functions for tests.      */
/*             : - Uses standard output stream for diagnostics.      */
/*             : - Intended for lightweight debugging and tracing.   */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef TEST_HELPERS_HPP
#define TEST_HELPERS_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <iostream>
#include <string>

/**
 * @brief Logs the result of a string conversion.
 *
 * Example:
 * Input  : "HELLO"
 * Output : "hello"
 */
inline void logConversion(const std::string &name, const std::string &input,
                          const std::string &output) {
  std::cout << "[" << name << "] Input: \"" << input << "\" => Output: \""
            << output << "\"" << std::endl;
}

#endif // TEST_HELPERS_HPP