// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : Client.cpp                                          */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Client                                         */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Maintains and executes string conversion strategy   */
/*               using the Strategy design pattern.                  */
/*                                                                   */
/* Notes       : - Uses std::unique_ptr for strategy ownership       */
/*               - Returns original input if strategy is not set     */
/*               - Supports optional trace ID for distributed tracing*/
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/* 1.1        2026-04-12  Nitish Singh    Added trace ID support     */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "Client.hpp"

void Client::setStrategy(std::unique_ptr<IStringConversion> s) {
  strategy = std::move(s);
}

ConversionResult Client::execute(const std::string &input) const {
  if (!strategy) {
    return ConversionResult(input.c_str());
  }

  return strategy->convert(input);
}

void Client::setTraceId(const std::string &traceId) { traceId_ = traceId; }