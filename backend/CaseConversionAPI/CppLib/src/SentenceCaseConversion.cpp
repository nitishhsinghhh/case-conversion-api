/*********************************************************************/
/* $File: SentenceCaseConversion.cpp                                 */
/*                                                                   */
/* Copyright (c) 2016-2026 nitishhsinghh. All rights reserved.       */
/* This material may be reproduced for teaching and learning         */
/* purposes only. It is not to be used in industry or for            */
/* commercial purposes.                                              */
/*                                                                   */
/* Class       - SentenceCaseConversion                              */
/*                                                                   */
/* Description - Converts input string to sentence case by           */
/*               capitalizing the first character and converting     */
/*               the remaining characters to lowercase.              */
/*                                                                   */
/* Notes       - Reuses LowerCaseConversion and UpperCaseConversion  */
/*               Handles empty input safely                          */
/*                                                                   */
/* $Log: SentenceCaseConversion.cpp                                  */
/*                                                                   */
/*  Revision 1.0  2026/04/11  Nitish Singh                           */
/*  Initial implementation of SentenceCaseConversion.                */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "SentenceCaseConversion.hpp"
#include "LowerCaseConversion.hpp"
#include "UpperCaseConversion.hpp"

ConversionResult
SentenceCaseConversion::convert(const std::string &input) const {
  if (input.empty()) {
    return ConversionResult(input.c_str());
  }

  LowerCaseConversion lowerConv;
  UpperCaseConversion upperConv;

  std::string result = ConversionResult(lowerConv.convert(input)).get_c_str();
  std::string first(1, result[0]);
  result[0] = ConversionResult(upperConv.convert(first)).get_c_str()[0];

  return ConversionResult(result.c_str());
}