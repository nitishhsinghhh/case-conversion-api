/* SPDX-License-Identifier: Apache-2.0 */

/*********************************************************************/
/* File          : lib.rs                                            */
/* Author        : Nitish Singh                                      */
/* Created       : 2026-06-07                                        */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Project       : Word Case API                                     */
/* Component     : Rust String Conversion Library                    */
/* Thread Safe   : Yes                                               */
/* API Status    : Stable (Production)                               */
/* Version       : 1.1.0                                             */
/*                                                                   */
/* Description   : Root module for the Rust string conversion engine.*/
/* Implements a polymorphic strategy pattern with                    */
/* dynamic routing between native Rust logic and                     */
/* legacy C++ modules via FFI.                                       */
/*                                                                   */
/* Architecture  :                                                   */
/*                                                                   */
/* [FFI Layer] ◄───► [Dispatcher (Hybrid Routing)]                   */
/* │                  │                                              */
/* ▼                  ▼                                              */
/* [Rust Engine]      [C++ Legacy Bridge]                            */
/* │                  │                                              */
/* └────────┬─────────┘                                              */
/* ▼                                                                 */
/* [Strategy Factory]                                                */
/* │                                                                 */
/* [Strategy Traits]                                                 */
/*                                                                   */
/* Design Patterns:                                                  */
/* : - Strategy Pattern (Execution)                                  */
/* : - Factory Pattern (Instantiation)                               */
/* : - Dispatcher Pattern (A/B Routing)                              */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07 Nitish Singh   Initial implementation       */
/* 1.1.0      2026-06-07 Nitish Singh   FFI dynamic routing & stable */
/*********************************************************************/

#![allow(non_snake_case)]

// --- Module Declarations ---
pub mod client;
pub mod conversion_type;
pub mod cpp_adapter;
pub mod dispatcher;
pub mod factory;
pub mod ffi;
pub mod rust_logic;
pub mod strategies; // This holds your #[no_mangle] extern "C" functions

// --- Public API Surface (Re-exports) ---
// This allows your tests to use `use rust_lib::StringConversionFactory;`
// instead of `use rust_lib::factory::StringConversionFactory;`
pub use crate::conversion_type::ConversionType;
pub use crate::dispatcher::process_string;
pub use crate::factory::StringConversionFactory;

// Export strategies so tests can find them at `rust_lib::LowerCaseConversion`
pub use crate::strategies::alternating::AlternatingCaseConversion;
pub use crate::strategies::capitalize::CapitalizeConversion;
pub use crate::strategies::kebab_case::KebabCaseConversion;
pub use crate::strategies::leetspeak::LeetSpeakConversion;
pub use crate::strategies::lowercase::LowerCaseConversion;
pub use crate::strategies::remove_spaces::RemoveSpacesConversion;
pub use crate::strategies::remove_vowels::RemoveVowelsConversion;
pub use crate::strategies::reverse::ReverseConversion;
pub use crate::strategies::sentence_case::SentenceCaseConversion;
pub use crate::strategies::snake_case::SnakeCaseConversion;
pub use crate::strategies::toggle_case::ToggleCaseConversion;
pub use crate::strategies::uppercase::UpperCaseConversion;
