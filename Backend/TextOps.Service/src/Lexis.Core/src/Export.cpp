// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : main.cpp                                            */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-05-20                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Text                                           */
/* Component   : Spell Check Engine Application                      */
/* Thread Safe : No                                                  */
/* Complexity  : N/A (Interactive main loop)                         */
/* API Status  : Internal                                            */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Interactive command-line executable wrapper for the */
/*               Lexis SpellChecker. Manages personal dictionary     */
/*               loading, Nuspell library validation, and execution  */
/*               control pipelines.                                  */
/*                                                                   */
/* Notes       : None.                                               */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0        2026-05-20  Nitish Singh    Initial implementation     */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "SpellChecker.hpp"
#include <iostream>
#include <string>
#include <limits>

int main() {
  using namespace Lexis::SpellCheck;
  SpellChecker checker;

  checker.LoadFromFile();

  if (!checker.LoadDictionary("/usr/share/hunspell/en_US")) {
    std::cout << "Warning: Could not load Nuspell dictionary. Suggestions disabled." << std::endl;
  }

  std::string input;
  while (true) {
    std::cout << "\nEnter word to spellcheck (or 'exit' to quit): ";
    std::cin >> input;

    if (input == "exit") {
      break; 
    }

    SpellResult result = checker.Check(input);

    if (result.isCorrect) {
      std::cout << "  [✓] '" << input << "' is spelled correctly (Nuspell)." << std::endl;
    } 
    else if (checker.Contains(input)) {
      std::cout << "  [✓] '" << input << "' found in your personal dictionary." << std::endl;
    }
    else {
      std::cout << "  [✗] '" << input << "' is misspelled!" << std::endl;
      
      if (!result.suggestions.empty()) {
        std::cout << "  Suggestions: ";
        for (size_t i = 0; i < result.suggestions.size() && i < 5; ++i) {
          std::cout << result.suggestions[i] << (i < 4 ? ", " : "");
        }
        std::cout << std::endl;
      }

      std::cout << "  Add '" << input << "' to your personal dictionary? (y/n): ";
      char choice;
      std::cin >> choice;
      
      if (choice == 'y' || choice == 'Y') {
        checker.Insert(input); 
        std::cout << "  [+] Saved to Personal Dictionary " << std::endl;
      }
      
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::cout << "\n[Exiting Lexis Engine... All changes saved. Goodbye!]" << std::endl;
  
  return 0;
}