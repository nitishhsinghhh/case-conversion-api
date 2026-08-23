// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : alternating.rs                                      */
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
/* Description : Alternating case conversion strategy.               */
/*               Converts alphabetic characters by alternating       */
/*               between uppercase and lowercase while preserving    */
/*               non-alphabetic characters unchanged.                */
/*                                                                   */
/* Example     : "hello world" -> "HeLlO wOrLd"                      */
/*             : "rust123"     -> "RuSt123"                          */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Alternation state maintained locally              */
/*             : - Non-alphabetic characters do not affect state     */
/*             : - UTF-8 input supported                             */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct AlternatingCaseConversion;

impl StringConversion for AlternatingCaseConversion {
    fn convert(&self, input: &str) -> String {
        let mut upper = true;

        input
            .chars()
            .map(|c| {
                if c.is_whitespace() {
                    upper = true;
                    return c;
                }

                if c.is_alphabetic() {
                    let out = if upper {
                        c.to_ascii_uppercase()
                    } else {
                        c.to_ascii_lowercase()
                    };

                    upper = !upper;
                    out
                } else {
                    c
                }
            })
            .collect()
    }
}
