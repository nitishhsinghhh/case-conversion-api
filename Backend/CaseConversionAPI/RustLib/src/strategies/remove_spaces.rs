// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : remove_spaces.rs                                    */
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
/* Description : Space removal conversion strategy.                  */
/*               Removes all space (' ') characters from the input   */
/*               string while preserving all other characters.       */
/*                                                                   */
/* Example     : "hello world"                                       */
/*                   -> "helloworld"                                 */
/*                                                                   */
/*             : "rust programming language"                         */
/*                   -> "rustprogramminglanguage"                    */
/*                                                                   */
/* Notes       : - Implements StringConversion strategy trait        */
/*             : - Uses Strategy Design Pattern                      */
/*             : - Removes only literal space characters (' ')       */
/*             : - Tabs and newlines are preserved                   */
/*             : - Preserves character ordering                      */
/*             : - UTF-8 compatible                                  */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

pub struct RemoveSpacesConversion;

impl StringConversion for RemoveSpacesConversion {
    fn convert(&self, input: &str) -> String {
        input.chars().filter(|c| *c != ' ').collect()
    }
}
