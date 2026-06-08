// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : mod.rs                                              */
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
/* Complexity  : O(1)                                                */
/* API Status  : Stable                                              */
/* Version     : 1.0.0                                               */
/*                                                                   */
/* Description : Defines the StringConversion strategy contract      */
/*               and exports all concrete string conversion          */
/*               strategy implementations.                           */
/*                                                                   */
/* Design      : Strategy Pattern                                    */
/*                                                                   */
/* Notes       : - Central strategy registry module                  */
/*             : - Provides common trait abstraction                 */
/*             : - Re-exports all strategy implementations           */
/*             : - Used by factory for runtime strategy creation     */
/*             : - Send + Sync for thread-safe usage                 */
/*             : - Supports dynamic dispatch via trait objects       */
/*                                                                   */
/* Exported Strategies:                                              */
/*             : - LowerCaseConversion                               */
/*             : - UpperCaseConversion                               */
/*             : - CapitalizeConversion                              */
/*             : - SentenceCaseConversion                            */
/*             : - ToggleCaseConversion                              */
/*             : - AlternatingCaseConversion                         */
/*             : - ReverseConversion                                 */
/*             : - RemoveVowelsConversion                            */
/*             : - RemoveSpacesConversion                            */
/*             : - InvertWordsConversion                             */
/*             : - SnakeCaseConversion                               */
/*             : - KebabCaseConversion                               */
/*             : - LeetSpeakConversion                               */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

pub trait StringConversion: Send + Sync {
    fn convert(&self, input: &str) -> String;
}

pub mod alternating;
pub mod capitalize;
pub mod invert_words;
pub mod kebab_case;
pub mod leetspeak;
pub mod lowercase;
pub mod remove_spaces;
pub mod remove_vowels;
pub mod reverse;
pub mod sentence_case;
pub mod snake_case;
pub mod toggle_case;
pub mod uppercase;

pub use alternating::AlternatingCaseConversion;
pub use capitalize::CapitalizeConversion;
pub use invert_words::InvertWordsConversion;
pub use kebab_case::KebabCaseConversion;
pub use leetspeak::LeetSpeakConversion;
pub use lowercase::LowerCaseConversion;
pub use remove_spaces::RemoveSpacesConversion;
pub use remove_vowels::RemoveVowelsConversion;
pub use reverse::ReverseConversion;
pub use sentence_case::SentenceCaseConversion;
pub use snake_case::SnakeCaseConversion;
pub use toggle_case::ToggleCaseConversion;
pub use uppercase::UpperCaseConversion;
