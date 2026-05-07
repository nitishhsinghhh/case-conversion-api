// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ProcessString.hpp                                   */
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
/* Description : Provides a client-facing interface for processing   */
/*               strings using a selected conversion strategy.       */
/*                                                                   */
/* Notes       : - Uses factory-based strategy resolution.           */
/*             : - Delegates conversion execution to Client layer.   */
/*             : - Returns structured ConversionResult objects.      */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef PROCESSSTRING_HPP
#define PROCESSSTRING_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "ConversionResult.hpp"
#include <string>

/**
 * @brief Processes a string using the specified conversion strategy.
 *
 * Example:
 * Input  : ("Hello World", 3)
 * Output : "hello world"
 */
ConversionResult processString(const std::string &input, int choice);

#endif // PROCESSSTRING_HPP