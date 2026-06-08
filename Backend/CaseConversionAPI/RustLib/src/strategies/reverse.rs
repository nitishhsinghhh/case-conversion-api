// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : reverse.rs                                          */
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
/* Description : Reverse string conversion strategy.                 */
/*               Reverses the order of characters in the input       */
/*               string while preserving character contents.         */
/*                                                                   */
/* Example     : "Hello World"                                       */
/*                   -> "dlroW olleH"                                */
/*                                                                   */
/*             : "Rust123"                                           */
/*                   -> "321tsuR"                                    */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Reverses character sequence                       */
/*             : - Preserves whitespace characters                   */
/*             : - Preserves punctuation characters                  */
/*             : - Unicode-aware character reversal                  */
/*             : - UTF-8 compatible                                  */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct ReverseConversion;

impl StringConversion for ReverseConversion {
    fn convert(&self, input: &str) -> String {
        input.chars().rev().collect()
    }
}
