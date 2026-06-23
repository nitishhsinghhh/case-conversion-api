// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ProcessStringDLL.cpp                                */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Interop                                        */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Strong Guarantee (externally guarded)          */
/*                                                                   */
/* Description : DLL wrapper exposing C++ string conversion engine   */
/*               for C# P/Invoke interoperability.                   */
/*               Delegates calls to core ProcessString dispatcher.   */
/*                                                                   */
/* Notes       : - Interop layer between native C++ and .NET API     */
/*             : - Enforces strict 5MB input size limit              */
/*             : - Uses heap allocation (malloc/free) for ABI safety */
/*             : - Caller (C# side) MUST free memory via freeString  */
/*             : - Returns fallback error strings on failure paths   */
/*             : - Catch-all exception guard prevents ABI leakage    */
/*             : - std::string constructed from raw input buffer     */
/*             : - Does NOT assume null-termination of input buffer  */
/*             : - TraceId is optional and used for observability    */
/*             : - Safe conversion mapping prevents invalid enum use */
/*             : - Designed for stable C ABI across language         */
/*                  boundary                                         */
/*                                                                   */
/* Memory Safety Notes:                                              */
/*             : - Every successful allocation has ownership         */
/*                 transfer                                          */
/*             : - No mixed delete/free (malloc paired with free)    */
/*             : - ConversionResult lifetime is local to function    */
/*             : - Output is deep-copied before returning to caller  */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-04-11  Nitish Singh    Initial implementation     */
/* 1.1        2026-04-12  Nitish Singh    Refactored for safety      */
/* 1.2        2026-04-13  Nitish Singh    Added 5MB security gate    */
/* 1.3        2026-04-18  Nitish Singh    Applied clang-format       */
/* 1.4        2026-04-18  Nitish Singh    Added traceId support      */
/* 1.5        2026-04-28  Nitish Singh    Hardened memory safety     */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "ProcessStringDLL.hpp"
#include "Client.hpp"
#include "ConversionTypeEnum.hpp"
#include "ProcessString.hpp"
#include "StringConversionFactory.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

/*********************************************************************/
/* Constants: 5 MB Buffer Limit: Hardcoded
/*********************************************************************/

namespace {
constexpr size_t MAX_INPUT_SIZE = 5 * 1024 * 1024;
}

/*********************************************************************/
/* Helper Utilities (internal, not exported - C++ only)
/*********************************************************************/

static char *allocateCString(const std::string &str) {
  char *output = static_cast<char *>(std::malloc(str.size() + 1));
  if (!output) {
    return nullptr;
  }

  std::memcpy(output, str.c_str(), str.size() + 1);
  return output;
}

static const char *safeError(const char *msg) {
  char *err = allocateCString(msg);
  return err ? err : "FATAL_ALLOCATION_FAILURE";
}

/*********************************************************************/
/* Conversion Mapping (Internal - C++ only)
/*********************************************************************/

static bool mapConversionType(ConversionChoice choice,
                              ConversionType &type) noexcept {
  switch (choice) {
  case ConversionChoice::Alternating:
    type = ConversionType::Alternating;
    return true;
  case ConversionChoice::Capitalize:
    type = ConversionType::Capitalize;
    return true;
  case ConversionChoice::Lower:
    type = ConversionType::Lower;
    return true;
  case ConversionChoice::Upper:
    type = ConversionType::Upper;
    return true;
  case ConversionChoice::Sentence:
    type = ConversionType::Sentence;
    return true;
  case ConversionChoice::Toggle:
    type = ConversionType::Toggle;
    return true;
  case ConversionChoice::Reverse:
    type = ConversionType::Reverse;
    return true;
  case ConversionChoice::RemoveVowels:
    type = ConversionType::RemoveVowels;
    return true;
  case ConversionChoice::RemoveSpaces:
    type = ConversionType::RemoveSpaces;
    return true;
  case ConversionChoice::InvertWords:
    type = ConversionType::InvertWords;
    return true;
  case ConversionChoice::SnakeCase:
    type = ConversionType::SnakeCase;
    return true;
  case ConversionChoice::KebabCase:
    type = ConversionType::KebabCase;
    return true;
  case ConversionChoice::LeetSpeak:
    type = ConversionType::LeetSpeak;
    return true;
  default:
    return false;
  }
}

/*********************************************************************/
/* Exported DLL API (Extern "C" for C# interop)
/*********************************************************************/

extern "C" {

API const char *processStringDLL(const char *input, int len, int choiceInt,
                                 const char *traceId) {
  try {
    if (!input) {
      return safeError("ERROR_NULL_INPUT");
    }

    size_t inputLength = static_cast<size_t>(len);
    if (inputLength > MAX_INPUT_SIZE) {
      return safeError("ERROR_BUFFER_OVERFLOW_LIMIT_5MB");
    }

    if (choiceInt < 0) {
      return safeError("ERROR_NEGATIVE_CONVERSION_CHOICE");
    }

    ConversionChoice choice = static_cast<ConversionChoice>(choiceInt);
    ConversionType type;

    if (!mapConversionType(choice, type)) {
      return safeError("ERROR_INVALID_CONVERSION_CHOICE");
    }

    Client client;
    if (traceId) {
      client.setTraceId(traceId);
    }

    client.setStrategy(StringConversionFactory::create(type));

    ConversionResult result = client.execute(std::string(input, inputLength));
    const char *rawPtr = result.get_c_str();

    if (!rawPtr) {
      return safeError("ERROR_ENGINE_RETURNED_NULL");
    }

    size_t resLen = std::strlen(rawPtr);
    char *output = static_cast<char *>(std::malloc(resLen + 1));

    if (!output) {
      return safeError("FATAL_ALLOCATION_FAILURE");
    }

    std::memcpy(output, rawPtr, resLen + 1);

    return output;

  } catch (...) {
    return safeError("ERROR_INTERNAL_EXCEPTION");
  }
}

API void freeString(char *str) {
  if (str) {
    std::free(str);
  }
}

} // extern "C"