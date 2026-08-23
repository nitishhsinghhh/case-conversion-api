// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : capitalize.rs                                       */
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
/* Description : Capitalization conversion strategy.                 */
/*               Converts the first character of every word to       */
/*               uppercase and converts remaining characters to      */
/*               lowercase.                                          */
/*                                                                   */
/* Example     : "hello world" -> "Hello World"                      */
/*             : "RUST PROGRAMMING" -> "Rust Programming"            */
/*             : "mIxEd CaSe" -> "Mixed Case"                        */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Words are separated by whitespace                 */
/*             : - Handles empty input safely                        */
/*             : - UTF-8 aware capitalization                        */
/*             : - Preserves word ordering                           */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct CapitalizeConversion;

impl StringConversion for CapitalizeConversion {
    fn convert(&self, input: &str) -> String {
        input
            .split_whitespace()
            .map(|word| {
                let mut chars = word.chars();
                match chars.next() {
                    Some(first) => {
                        first.to_uppercase().collect::<String>() + &chars.as_str().to_lowercase()
                    }
                    None => String::new(),
                }
            })
            .collect::<Vec<_>>()
            .join(" ")
    }
}
