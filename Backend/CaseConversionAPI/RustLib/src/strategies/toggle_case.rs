// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : toggle_case.rs                                      */
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
/* Description : Toggle case conversion strategy.                    */
/*               Inverts the case of each ASCII alphabetic           */
/*               character in the input string.                      */
/*                                                                   */
/* Example     : "Hello World"                                       */
/*                   -> "hELLO wORLD"                                */
/*                                                                   */
/*             : "RuSt123"                                           */
/*                   -> "rUsT123"                                    */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Lowercase letters become uppercase                */
/*             : - Uppercase letters become lowercase                */
/*             : - Digits and punctuation are preserved              */
/*             : - Non-alphabetic characters are unchanged           */
/*             : - UTF-8 compatible                                  */
/*                                                                   */
/* Limitations : - Uses ASCII-only case conversion                   */
/*             : - Non-ASCII alphabetic characters are not toggled   */
/*             :   (e.g. é, ü, Ω remain unchanged)                   */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct ToggleCaseConversion;

impl StringConversion for ToggleCaseConversion {
    fn convert(&self, input: &str) -> String {
        input
            .chars()
            .map(|c| {
                if c.is_ascii_lowercase() {
                    c.to_ascii_uppercase()
                } else if c.is_ascii_uppercase() {
                    c.to_ascii_lowercase()
                } else {
                    c
                }
            })
            .collect()
    }
}
