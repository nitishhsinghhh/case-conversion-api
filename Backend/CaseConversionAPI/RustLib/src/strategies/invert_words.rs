// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : invert_words.rs                                     */
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
/* Description : Word inversion conversion strategy.                 */
/*               Reverses the order of words in the input string     */
/*               while preserving the original word contents.        */
/*                                                                   */
/* Example     : "hello world rust"                                  */
/*                   -> "rust world hello"                           */
/*                                                                   */
/*             : "one two three four"                                */
/*                   -> "four three two one"                         */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Words are separated by whitespace                 */
/*             : - Preserves individual word casing                  */
/*             : - Preserves individual word contents                */
/*             : - UTF-8 compatible                                  */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : invert_words.rs                                     */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-07                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/String                                         */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Version     : 1.0.0                                               */
/*                                                                   */
/* Description : Reverses characters of each word in the input       */
/*               string while preserving original word order.        */
/*                                                                   */
/* Notes       : - Splits input into whitespace-delimited words      */
/*             : - Reverses each word independently                  */
/*             : - Preserves word ordering                           */
/*             : - Consecutive whitespace normalized to single       */
/*                 spaces, matching C++ implementation               */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct InvertWordsConversion;

impl StringConversion for InvertWordsConversion {
    fn convert(&self, input: &str) -> String {
        input
            .split_whitespace()
            .map(|word| word.chars().rev().collect::<String>())
            .collect::<Vec<_>>()
            .join(" ")
    }
}
