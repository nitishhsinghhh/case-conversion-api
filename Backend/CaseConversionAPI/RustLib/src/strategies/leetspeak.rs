// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : leetspeak.rs                                        */
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
/* Description : LeetSpeak conversion strategy.                      */
/*               Converts selected alphabetic characters into        */
/*               common LeetSpeak (1337) numeric substitutions.      */
/*                                                                   */
/* Example     : "Leet Speak"                                        */
/*                   -> "L337 5p34k"                                 */
/*                                                                   */
/*             : "Rust Language"                                     */
/*                   -> "Ru57 L4ngu4g3"                              */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Character mapping is case-insensitive             */
/*             : - Preserves non-mapped characters unchanged         */
/*             : - UTF-8 compatible                                  */
/*             : - Current substitutions:                            */
/*             :     a -> 4                                          */
/*             :     e -> 3                                          */
/*             :     i -> 1                                          */
/*             :     o -> 0                                          */
/*             :     s -> 5                                          */
/*             :     t -> 7                                          */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct LeetSpeakConversion;

impl StringConversion for LeetSpeakConversion {
    fn convert(&self, input: &str) -> String {
        input
            .chars()
            .map(|c| match c.to_ascii_lowercase() {
                'a' => '4',
                'e' => '3',
                'i' => '1',
                'o' => '0',
                's' => '5',
                't' => '7',
                _ => c,
            })
            .collect()
    }
}
