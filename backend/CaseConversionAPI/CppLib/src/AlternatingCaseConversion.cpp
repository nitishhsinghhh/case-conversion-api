/*********************************************************************/
/* $File:: AlternatingCaseConversion.cpp                             */
/*                                                                   */
/* Copyright (c) 2016-2026 nitishhsinghh. All rights reserved.       */
/* This material may be reproduced for teaching and learning         */
/* purposes only. It is not to be used in industry or for            */
/* commercial purposes.                                              */
/*                                                                   */
/* Class       - AlternatingCaseConversion                           */
/*                                                                   */
/* Description - Converts input string into alternating case format  */
/*               starting with uppercase for each word.              */
/*                                                                   */
/* Notes       - Reuses LowerCaseConversion and                      */
/*               UpperCaseConversion                                 */
/*               Alternation resets after spaces                     */
/*                                                                   */
/* $Log: AlternatingCaseConversion.cpp                               */
/*                                                                   */
/* Revision 1.0  2026/04/11  Nitish Singh                            */
/* Initial implementation of AlternatingCaseConversion.              */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "AlternatingCaseConversion.hpp"
#include "ConversionResult.hpp"
#include "LowerCaseConversion.hpp"
#include "UpperCaseConversion.hpp"

ConversionResult
AlternatingCaseConversion::convert(const std::string &input) const {
  LowerCaseConversion lowerConv;
  UpperCaseConversion upperConv;

  std::string finalResult; // Renamed for clarity
  bool upper = true;

  for (char c : input) {
    // Standardize check using isalpha for better practice
    if (std::isalpha(static_cast<unsigned char>(c))) {
      std::string temp(1, c);

      if (upper) {
        auto resObj = upperConv.convert(temp);
        finalResult += resObj.get_c_str();
      } else {
        auto resObj = lowerConv.convert(temp);
        finalResult += resObj.get_c_str();
      }

      upper = !upper;
    } else {
      finalResult += c;
      if (c == ' ') {
        upper = true;
      }
    }
  }

  return ConversionResult(finalResult.c_str());
}