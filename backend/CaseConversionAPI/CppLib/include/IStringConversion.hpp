// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : IStringConversion.hpp                               */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Interfaces                                     */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n) (Implementation Dependent)                     */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Defines the abstract contract for all string        */
/*               conversion strategies used throughout the engine.   */
/*                                                                   */
/* Notes       : - Base interface for all transformation strategies. */
/*             : - Used by the Strategy pattern context layer.       */
/*             : - Supports polymorphic conversion execution.        */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial interface design   */
/*********************************************************************/

#ifndef ISTRINGCONVERSION_HPP
#define ISTRINGCONVERSION_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "ConversionResult.hpp"
#include <string>

/**
 * @interface IStringConversion
 * @brief Interface for string conversion strategies.
 *
 * This interface defines a common API for all string conversion strategies.
 */
class IStringConversion {
public:
  /**
   * @brief Converts the input string according to the strategy.
   * @param input The input string to be converted.
   * @return The converted string.
   */
  virtual ConversionResult convert(const std::string &input) const = 0;

  /**
   * @brief Virtual destructor for proper cleanup of derived classes.
   */
  virtual ~IStringConversion() = default;
};

#endif // ISTRINGCONVERSION_HPP