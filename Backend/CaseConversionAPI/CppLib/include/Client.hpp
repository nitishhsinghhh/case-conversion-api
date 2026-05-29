// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : Client.hpp                                          */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Strategy                                       */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : No                                                  */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Context class implementing the Strategy design      */
/*               pattern for executing string conversion strategies. */
/*                                                                   */
/* Notes       : Delegates conversion logic to                       */
/*               IStringConversion implementations.                  */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/* 1.1        2026-04-12  Nitish Singh    Added distributed tracing  */
/*********************************************************************/

#ifndef CLIENT_HPP
#define CLIENT_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <memory>
#include <string>

/**
 * @class Client
 * @brief Context class for executing string conversion strategies.
 *
 * Example:
 * auto strategy = std::make_unique<LowerCaseConversion>();
 * Client client;
 * client.setStrategy(std::move(strategy));
 * ConversionResult result = client.execute("HELLO"); // "hello"
 */
class Client {
private:
  /// Pointer to the current string conversion strategy
  std::unique_ptr<IStringConversion> strategy;
  std::string traceId_; // For distributed tracing (OpenTelemetry)

public:
  /**
   * @brief Sets the current string conversion strategy.
   * @param s A unique pointer to an IStringConversion implementation.
   */
  void setStrategy(std::unique_ptr<IStringConversion> s);

  /**
   * @brief Executes the current strategy on the given input string.
   * @param input The string to convert.
   * @return The converted string according to the current strategy.
   *         Returns original input if no strategy is set.
   */
  ConversionResult execute(const std::string &input) const;

  void setTraceId(const std::string &traceId);
};

#endif // CLIENT_HPP