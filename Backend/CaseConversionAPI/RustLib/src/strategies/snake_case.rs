// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : snake_case.rs                                       */
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
/* Description : Snake case conversion strategy.                     */
/*               Converts input text to lowercase and replaces       */
/*               space characters with underscores ('_').            */
/*                                                                   */
/* Example     : "Hello World"                                       */
/*                   -> "hello_world"                                */
/*                                                                   */
/*             : "Rust Programming Language"                         */
/*                   -> "rust_programming_language"                  */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Converts all alphabetic characters to lowercase   */
/*             : - Replaces space characters with underscores        */
/*             : - Preserves punctuation characters                  */
/*             : - UTF-8 compatible                                  */
/*             : - Consecutive spaces become consecutive             */
/*                 underscores                                       */
/*                                                                   */
/* Limitations : - Handles only literal space characters (' ')       */
/*             : - Does not normalize tabs or newlines               */
/*             : - Does not convert camelCase or PascalCase          */
/*             : - Intended as a simple snake_case transformation    */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct SnakeCaseConversion;

impl StringConversion for SnakeCaseConversion {
    fn convert(&self, input: &str) -> String {
        input.to_lowercase().replace(' ', "_")
    }
}
