// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : uppercase.rs                                        */
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
/* Description : Uppercase conversion strategy.                      */
/*               Converts all alphabetic characters in the input     */
/*               string to their uppercase equivalents.              */
/*                                                                   */
/* Example     : "hello world"                                       */
/*                   -> "HELLO WORLD"                                */
/*                                                                   */
/*             : "Rust Programming"                                  */
/*                   -> "RUST PROGRAMMING"                           */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Preserves non-alphabetic characters               */
/*             : - Unicode-aware uppercase conversion                */
/*             : - UTF-8 compatible                                  */
/*             : - Safe for multi-byte Unicode characters            */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct UpperCaseConversion;

impl StringConversion for UpperCaseConversion {
    fn convert(&self, input: &str) -> String {
        input.to_uppercase()
    }
}
