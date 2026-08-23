// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : conversion_type.rs                                  */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-07                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core                                                */
/* Component   : Case Conversion Engine                              */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(1)                                                */
/* API Status  : Stable                                              */
/* Version     : 1.0.0                                               */
/*                                                                   */
/* Description : Enumerates all supported string conversion          */
/*               operations available in the conversion engine.      */
/*                                                                   */
/* Design      : Factory Pattern Support                             */
/*                                                                   */
/* Notes       : - Used by StringConversionFactory                   */
/*             : - Provides type-safe conversion selection           */
/*             : - Prevents invalid strategy construction            */
/*             : - Supports lightweight copying via Clone + Copy     */
/*             : - Used by dispatcher and interop layers             */
/*                                                                   */
/* Supported Conversions:                                            */
/*             : - Alternating                                       */
/*             : - Capitalize                                        */
/*             : - Lower                                             */
/*             : - Upper                                             */
/*             : - Sentence                                          */
/*             : - Toggle                                            */
/*             : - Reverse                                           */
/*             : - RemoveVowels                                      */
/*             : - RemoveSpaces                                      */
/*             : - InvertWords                                       */
/*             : - SnakeCase                                         */
/*             : - KebabCase                                         */
/*             : - LeetSpeak                                         */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

/// Enumeration representing all supported string conversion
/// operations within the conversion engine.
#[repr(i32)]
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum ConversionType {
    Alternating = 1,
    Capitalize = 2,
    Lower = 3,
    Upper = 4,
    Sentence = 5,
    Toggle = 6,
    Reverse = 7,
    RemoveVowels = 8,
    RemoveSpaces = 9,
    InvertWords = 10,
    SnakeCase = 11,
    KebabCase = 12,
    LeetSpeak = 13,
}
