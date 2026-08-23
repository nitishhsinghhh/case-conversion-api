// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : factory.rs                                          */
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
/* Description : Factory responsible for constructing concrete       */
/*               string conversion strategy objects at runtime.      */
/*               Encapsulates strategy creation logic and            */
/*               decouples callers from implementation details.      */
/*                                                                   */
/* Design      : Factory Pattern                                     */
/*                                                                   */
/* Notes       : - Creates strategy instances on demand             */
/*             : - Returns boxed trait objects                      */
/*             : - Hides concrete implementation types              */
/*             : - Supports runtime strategy selection              */
/*             : - Used by dispatcher layer                         */
/*             : - Works with Strategy Pattern architecture         */
/*                                                                   */
/* Supported Strategies:                                             */
/*             : - AlternatingCaseConversion                        */
/*             : - CapitalizeConversion                             */
/*             : - LowerCaseConversion                              */
/*             : - UpperCaseConversion                              */
/*             : - SentenceCaseConversion                           */
/*             : - ToggleCaseConversion                             */
/*             : - ReverseConversion                                */
/*             : - RemoveVowelsConversion                           */
/*             : - RemoveSpacesConversion                           */
/*             : - InvertWordsConversion                            */
/*             : - SnakeCaseConversion                              */
/*             : - KebabCaseConversion                              */
/*             : - LeetSpeakConversion                              */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::conversion_type::ConversionType;
use crate::strategies::*;

/// Factory for creating string conversion strategies.
pub struct StringConversionFactory;

impl StringConversionFactory {
    /// Creates a concrete strategy instance corresponding
    /// to the supplied conversion type.
    pub fn create(conversion: ConversionType) -> Box<dyn StringConversion> {
        match conversion {
            ConversionType::Alternating => Box::new(AlternatingCaseConversion),

            ConversionType::Capitalize => Box::new(CapitalizeConversion),

            ConversionType::Lower => Box::new(LowerCaseConversion),

            ConversionType::Upper => Box::new(UpperCaseConversion),

            ConversionType::Sentence => Box::new(SentenceCaseConversion),

            ConversionType::Toggle => Box::new(ToggleCaseConversion),

            ConversionType::Reverse => Box::new(ReverseConversion),

            ConversionType::RemoveVowels => Box::new(RemoveVowelsConversion),

            ConversionType::RemoveSpaces => Box::new(RemoveSpacesConversion),

            ConversionType::InvertWords => Box::new(InvertWordsConversion),

            ConversionType::SnakeCase => Box::new(SnakeCaseConversion),

            ConversionType::KebabCase => Box::new(KebabCaseConversion),

            ConversionType::LeetSpeak => Box::new(LeetSpeakConversion),
        }
    }
}
