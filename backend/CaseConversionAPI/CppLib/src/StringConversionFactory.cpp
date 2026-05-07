// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : StringConversionFactory.cpp                         */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Factory                                        */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(1) dispatch                                       */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Factory class responsible for creating string       */
/*               conversion strategy objects based on type.          */
/*                                                                   */
/* Notes       : - Implements Factory design pattern                 */
/*             : - Returns std::unique_ptr<IStringConversion>        */
/*             : - Centralized strategy creation point               */
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

#include "StringConversionFactory.hpp"
#include "AlternatingCaseConversion.hpp"
#include "CapitalizeWordsConversion.hpp"
#include "InvertWordsConversion.hpp"
#include "KebabCaseConversion.hpp"
#include "LeetSpeakConversion.hpp"
#include "LowerCaseConversion.hpp"
#include "RemoveSpacesConversion.hpp"
#include "RemoveVowelsConversion.hpp"
#include "ReverseConversion.hpp"
#include "SentenceCaseConversion.hpp"
#include "SnakeCaseConversion.hpp"
#include "ToggleCaseConversion.hpp"
#include "UpperCaseConversion.hpp"

std::unique_ptr<IStringConversion>
StringConversionFactory::create(ConversionType type) {
  switch (type) {
  case ConversionType::Lower:
    return std::make_unique<LowerCaseConversion>();
  case ConversionType::Upper:
    return std::make_unique<UpperCaseConversion>();
  case ConversionType::Capitalize:
    return std::make_unique<CapitalizeWordsConversion>();
  case ConversionType::Sentence:
    return std::make_unique<SentenceCaseConversion>();
  case ConversionType::Toggle:
    return std::make_unique<ToggleCaseConversion>();
  case ConversionType::Alternating:
    return std::make_unique<AlternatingCaseConversion>();
  case ConversionType::Reverse:
    return std::make_unique<ReverseConversion>();
  case ConversionType::RemoveVowels:
    return std::make_unique<RemoveVowelsConversion>();
  case ConversionType::RemoveSpaces:
    return std::make_unique<RemoveSpacesConversion>();
  case ConversionType::InvertWords:
    return std::make_unique<InvertWordsConversion>();
  case ConversionType::SnakeCase:
    return std::make_unique<SnakeCaseConversion>();
  case ConversionType::KebabCase:
    return std::make_unique<KebabCaseConversion>();
  case ConversionType::LeetSpeak:
    return std::make_unique<LeetSpeakConversion>();
  default:
    break;
  }

  return nullptr; // fallback
}