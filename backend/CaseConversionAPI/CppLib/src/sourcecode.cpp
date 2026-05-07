// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : sourcecode.cpp (main file)                          */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-04-11                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/CLI                                            */
/* Component   : Test Application                                    */
/* Thread Safe : No (single-threaded console application)            */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Console application for local testing of string     */
/*               conversion engine. Uses ProcessString dispatcher.   */
/*               Accepts user input and conversion choice, then      */
/*               displays the converted result.                      */
/*                                                                   */
/* Notes       : - Uses ProcessString dispatcher                     */
/*             : - Demonstrates runtime selection of conversions     */
/*             : - Simple CLI driver for validation/testing         */
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

#include "ProcessString.hpp"
#include <iostream>

int main() {
  std::string input;

  std::cout << "Enter string: ";
  std::getline(std::cin, input);

  std::cout << "1: Alternating, 2: Capitalize, 3: Lower, 4: Upper, "
               "5: Sentence, 6: Toggle, 7: Reverse, 8: RemoveVowels, "
               "9: RemoveSpaces, 10: InvertWords, 11: SnakeCase, "
               "12: KebabCase, 13: LeetSpeak\n";

  int choice;
  std::cin >> choice;

  ConversionResult result = processString(input, choice);

  std::cout << "Result: " << result.get_c_str() << std::endl;

  return 0;
}