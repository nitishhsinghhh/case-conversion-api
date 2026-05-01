/*********************************************************************/
/* $File: CapitalizeWordsConversion.cpp                              */
/*                                                                   */
/* Copyright (c) 2016-2026 nitishhsinghh. All rights reserved.       */
/* This material may be reproduced for teaching and learning         */
/* purposes only. It is not to be used in industry or for            */
/* commercial purposes.                                              */
/*                                                                   */
/* Class       - CapitalizeWordsConversion                           */
/*                                                                   */
/* Description - Converts input string so that each word starts      */
/*               with an uppercase letter followed by lowercase      */
/*               characters.                                         */
/*                                                                   */
/* Notes       - Reuses LowerCaseConversion and                      */
/*               UpperCaseConversion                                 */
/*                                                                   */
/* $Log: CapitalizeWordsConversion.cpp                               */
/*                                                                   */
/*  Revision 1.0  2026/04/11  Nitish Singh                           */
/*  Initial implementation of CapitalizeWordsConversion.             */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "CapitalizeWordsConversion.hpp"
#include "LowerCaseConversion.hpp"
#include "UpperCaseConversion.hpp"
#include <sstream>

ConversionResult
CapitalizeWordsConversion::convert(const std::string &input) const {
  LowerCaseConversion lowerConv;
  UpperCaseConversion upperConv;

  std::istringstream iss(input);
  std::string word;
  std::string result;

  while (iss >> word) {
    if (!result.empty()) {
      result += " ";
    }

    // Convert whole word to lowercase first
    word = ConversionResult(lowerConv.convert(word))
               .get_c_str(); // Get C-string from ConversionResult

    // Capitalize first letter using UpperCaseConversion
    std::string firstChar(1, word[0]);
    firstChar = ConversionResult(upperConv.convert(firstChar)).get_c_str();
    word[0] = firstChar[0];

    result += word;
  }

  return ConversionResult(result.c_str());
}