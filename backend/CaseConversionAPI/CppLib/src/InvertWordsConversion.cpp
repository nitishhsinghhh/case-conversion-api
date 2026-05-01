/*********************************************************************/
/* $File: InvertWordsConversion.cpp                                  */
/*                                                                   */
/* Copyright (c) 2016-2026 nitishhsinghh. All rights reserved.       */
/* This material may be reproduced for teaching and learning         */
/* purposes only. It is not to be used in industry or for            */
/* commercial purposes.                                              */
/*                                                                   */
/* Class       - InvertWordsConversion                               */
/*                                                                   */
/* Description - Reverses characters of each word in the input       */
/*               string while preserving word order.                 */
/*                                                                   */
/* Notes       - Uses string stream to split words                   */
/*               Uses std::reverse for in-place word reversal        */
/*                                                                   */
/* $Log: InvertWordsConversion.cpp                                   */
/*                                                                   */
/*  Revision 1.0  2026/04/11  Nitish Singh                           */
/*  Initial implementation of InvertWordsConversion.                 */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/
#include "InvertWordsConversion.hpp"
#include <algorithm>
#include <sstream>

ConversionResult
InvertWordsConversion::convert(const std::string &input) const {
  std::istringstream iss(input);
  std::ostringstream oss;
  std::string word;
  bool first = true;

  while (iss >> word) {
    std::reverse(word.begin(), word.end());

    if (!first) {
      oss << ' ';
    }

    oss << word;
    first = false;
  }

  return ConversionResult(oss.str().c_str());
}