// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : dispatcher.rs                                       */
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
/* Complexity  : O(1) + Strategy Complexity                          */
/* API Status  : Stable                                              */
/* Version     : 1.0.0                                               */
/*                                                                   */
/* Description : Central dispatcher for string conversion requests.  */
/*               Maps conversion choices to conversion types,        */
/*               creates the appropriate strategy via the factory,   */
/*               and executes the conversion through the client.     */
/*                                                                   */
/* Design      : Dispatcher + Factory + Strategy Pattern             */
/*                                                                   */
/* Notes       : - Validates conversion choice input                 */
/*             : - Provides type-safe enum mapping                   */
/*             : - Delegates strategy creation to factory            */
/*             : - Delegates execution to client context             */
/*             : - Returns standardized error messages              */
/*             : - Used by DLL interop layer                         */
/*                                                                   */
/* Supported Choices:                                                */
/*             :  1 -> Alternating                                   */
/*             :  2 -> Capitalize                                    */
/*             :  3 -> Lower                                         */
/*             :  4 -> Upper                                         */
/*             :  5 -> Sentence                                      */
/*             :  6 -> Toggle                                        */
/*             :  7 -> Reverse                                       */
/*             :  8 -> RemoveVowels                                  */
/*             :  9 -> RemoveSpaces                                  */
/*             : 10 -> InvertWords                                   */
/*             : 11 -> SnakeCase                                     */
/*             : 12 -> KebabCase                                     */
/*             : 13 -> LeetSpeak                                     */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::client::Client;
use crate::conversion_type::ConversionType;
use crate::factory::StringConversionFactory;

/// Maps an integer conversion choice to a strongly typed
/// ConversionType enumeration.
///
/// Returns None when an invalid choice is supplied.
fn map_choice(choice: i32) -> Option<ConversionType> {
    match choice {
        1 => Some(ConversionType::Alternating),
        2 => Some(ConversionType::Capitalize),
        3 => Some(ConversionType::Lower),
        4 => Some(ConversionType::Upper),
        5 => Some(ConversionType::Sentence),
        6 => Some(ConversionType::Toggle),
        7 => Some(ConversionType::Reverse),
        8 => Some(ConversionType::RemoveVowels),
        9 => Some(ConversionType::RemoveSpaces),
        10 => Some(ConversionType::InvertWords),
        11 => Some(ConversionType::SnakeCase),
        12 => Some(ConversionType::KebabCase),
        13 => Some(ConversionType::LeetSpeak),
        _ => None,
    }
}

/// Processes a string conversion request.
///
/// # Arguments
///
/// * `input` - Source string.
/// * `choice` - Numeric conversion selection.
///
/// # Returns
///
/// * `Ok(String)` - Converted output.
/// * `Err(&'static str)` - Standardized error code.
pub fn process_string(input: &str, choice: i32) -> Result<String, &'static str> {
    let conversion = map_choice(choice).ok_or("ERROR_INVALID_CONVERSION_CHOICE")?;

    let strategy = StringConversionFactory::create(conversion);

    let client = Client::new(strategy);

    Ok(client.execute(input))
}
