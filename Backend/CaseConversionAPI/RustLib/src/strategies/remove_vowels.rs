// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : remove_vowels.rs                                    */
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
/* Description : Vowel removal conversion strategy.                  */
/*               Removes all English vowels from the input string    */
/*               while preserving consonants, digits, punctuation,   */
/*               and whitespace characters.                          */
/*                                                                   */
/* Example     : "Hello World"                                       */
/*                   -> "Hll Wrld"                                   */
/*                                                                   */
/*             : "Rust Programming"                                  */
/*                   -> "Rst Prgrmmng"                               */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Removes vowels: a, e, i, o, u                     */
/*             : - Case-insensitive vowel matching                   */
/*             : - Preserves character ordering                      */
/*             : - Preserves whitespace and punctuation              */
/*             : - UTF-8 compatible                                  */
/*             : - Non-English vowels are not removed                */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct RemoveVowelsConversion;

impl StringConversion for RemoveVowelsConversion {
    fn convert(&self, input: &str) -> String {
        input
            .chars()
            .filter(|c| !"aeiouAEIOU".contains(*c))
            .collect()
    }
}
