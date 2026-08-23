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
/*               spaces with underscore characters ('_').            */
/*                                                                   */
/* Example     : "Hello World"                                       */
/*                   -> "hello-world"                                */
/*                                                                   */
/*             : "Rust Programming Language"                         */
/*                   -> "rust-programming-language"                  */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Converts all alphabetic characters to lowercase   */
/*             : - Replaces space characters with underscores        */
/*             : - Preserves non-space punctuation characters        */
/*             : - UTF-8 compatible                                  */
/*             : - Consecutive spaces produce consecutive            */
/*                 underscores in output                             */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct KebabCaseConversion;

impl StringConversion for KebabCaseConversion {
    fn convert(&self, input: &str) -> String {
        input.to_lowercase().replace(' ', "-")
    }
}
