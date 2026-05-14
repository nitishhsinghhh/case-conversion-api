// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : StringConversionFactory.hpp                         */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Factory                                        */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(1)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Strong Guarantee                               */
/*                                                                   */
/* Description : Defines the factory interface responsible for       */
/*               creating concrete string conversion strategy        */
/*               objects based on ConversionType selection.          */
/*                                                                   */
/* Notes       : - Implements Factory Method design pattern.         */
/*             : - Returns polymorphic conversion strategy objects.  */
/*             : - Uses std::unique_ptr for automatic ownership.     */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/*********************************************************************/

#ifndef STRINGCONVERSIONFACTORY_HPP
#define STRINGCONVERSIONFACTORY_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "IStringConversion.hpp"
#include <memory>

/**
 * @enum ConversionType
 * @brief Defines types of string conversions available.
 */
enum class ConversionType {
  Lower,
  Upper,
  Capitalize,
  Sentence,
  Toggle,
  Alternating,
  Reverse,
  RemoveVowels,
  RemoveSpaces,
  InvertWords,
  SnakeCase,
  KebabCase,
  LeetSpeak
};

/**
 * @class StringConversionFactory
 * @brief Factory class to create IStringConversion objects.
 *
 * Example:
 * Input  : ConversionType::Upper
 * Output : "HELLO"
 */
class StringConversionFactory {
public:
  /**
   * @brief Creates a string conversion object based on ConversionType.
   * @param type The type of conversion to create.
   * @return Unique pointer to conversion object.
   */
  static std::unique_ptr<IStringConversion> create(ConversionType type);
};

#endif // STRINGCONVERSIONFACTORY_HPP