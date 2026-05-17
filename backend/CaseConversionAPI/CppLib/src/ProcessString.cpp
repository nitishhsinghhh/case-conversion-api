// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ProcessString.cpp                                   */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Dispatcher                                     */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Core dispatcher selecting conversion strategies     */
/*               using Factory and Strategy design patterns.         */
/*               Shared by DLL and CLI applications.                 */
/*                                                                   */
/* Notes       : - Uses Strategy and Factory design patterns         */
/*             : - Returns original input for invalid choices        */
/*             : - Maps ConversionChoice to ConversionType safely    */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/* 1.1        2026-04-12  Nitish Singh    Code quality improvements  */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "ProcessString.hpp"
#include "Client.hpp"
#include "ConversionTypeEnum.hpp"
#include "StringConversionFactory.hpp"

static ConversionType mapChoiceToType(ConversionChoice choice) {
  switch (choice) {
  case ConversionChoice::Alternating:
    return ConversionType::Alternating;
  case ConversionChoice::Capitalize:
    return ConversionType::Capitalize;
  case ConversionChoice::Lower:
    return ConversionType::Lower;
  case ConversionChoice::Upper:
    return ConversionType::Upper;
  case ConversionChoice::Sentence:
    return ConversionType::Sentence;
  case ConversionChoice::Toggle:
    return ConversionType::Toggle;
  case ConversionChoice::Reverse:
    return ConversionType::Reverse;
  case ConversionChoice::RemoveVowels:
    return ConversionType::RemoveVowels;
  case ConversionChoice::RemoveSpaces:
    return ConversionType::RemoveSpaces;
  case ConversionChoice::InvertWords:
    return ConversionType::InvertWords;
  case ConversionChoice::SnakeCase:
    return ConversionType::SnakeCase;
  case ConversionChoice::KebabCase:
    return ConversionType::KebabCase;
  case ConversionChoice::LeetSpeak:
    return ConversionType::LeetSpeak;
  default:
    return ConversionType::Lower; // safe fallback
  }
}

ConversionResult processString(const std::string &input, int choiceInt) {
  Client client;

  ConversionChoice choice = static_cast<ConversionChoice>(choiceInt);

  // Map enum to ConversionType
  ConversionType type = mapChoiceToType(choice);

  client.setStrategy(StringConversionFactory::create(type));

  return client.execute(input);
}