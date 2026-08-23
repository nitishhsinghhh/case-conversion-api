// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : sentence_case.rs                                    */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-07                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Strategies                                     */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Version     : 1.0.0                                               */
/*                                                                   */
/* Description : Sentence case conversion strategy.                  */
/*               Converts the first character of the input string    */
/*               to uppercase and converts all remaining characters  */
/*               to lowercase.                                       */
/*                                                                   */
/* Example     : "hELLO WORLD"                                       */
/*                   -> "Hello world"                                */
/*                                                                   */
/*             : "RUST PROGRAMMING LANGUAGE"                         */
/*                   -> "Rust programming language"                  */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Entire input is normalized to lowercase first     */
/*             : - First character is then capitalized               */
/*             : - Empty input is handled safely                     */
/*             : - Unicode-aware case conversion                     */
/*             : - UTF-8 compatible                                  */
/*                                                                   */
/* Limitations : - Only the first character of the entire string     */
/*                 is capitalized                                    */
/*             : - Does not detect multiple sentences separated      */
/*                 by '.', '!' or '?'                                */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct SentenceCaseConversion;

impl StringConversion for SentenceCaseConversion {
    fn convert(&self, input: &str) -> String {
        let lower = input.to_lowercase();

        if lower.is_empty() {
            return lower;
        }

        let mut chars = lower.chars();
        let first = chars.next().unwrap();

        first.to_uppercase().collect::<String>() + chars.as_str()
    }
}
