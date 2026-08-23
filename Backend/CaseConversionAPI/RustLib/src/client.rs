// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : client.rs                                           */
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
/* Description : Client context for the Strategy Pattern.            */
/*               Encapsulates a string conversion strategy and       */
/*               delegates execution to the selected strategy        */
/*               implementation at runtime.                          */
/*                                                                   */
/* Design      : Strategy Pattern                                    */
/*                                                                   */
/* Notes       : - Owns a boxed strategy instance                    */
/*             : - Uses dynamic dispatch via trait objects           */
/*             : - Decouples caller from concrete strategies         */
/*             : - Thread-safe through Send + Sync trait bounds      */
/*             : - Execution complexity depends on strategy          */
/*                                                                   */
/* Usage Flow  : Factory -> Client -> Strategy -> Result             */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author          Description                */
/* ----------------------------------------------------------------- */
/* 1.0.0      2026-06-07  Nitish Singh    Initial implementation     */
/*********************************************************************/

use crate::strategies::StringConversion;

/// Strategy Pattern Context.
///
/// Holds the selected string conversion strategy and
/// delegates conversion requests to it.
pub struct Client {
    strategy: Box<dyn StringConversion>,
}

impl Client {
    /// Creates a new client with the specified strategy.
    pub fn new(strategy: Box<dyn StringConversion>) -> Self {
        Self { strategy }
    }

    /// Executes the configured conversion strategy.
    pub fn execute(&self, input: &str) -> String {
        self.strategy.convert(input)
    }
}
