//--------------------------------------------------------------------------------------------------
// File        : BasicConversionTests.cs
// Author      : Nitish Singh
// Version     : 1.1
// License     : Apache License, Version 2.0 
// Copyright   : (c) 2016–2026 Nitish Singh. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except 
// in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the 
// License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND.
//
//
// Change History (Reverse Chronological):
// 1.1 | 2026-05-09 | Nitish Singh | Upgraded to XML Documentation for IDE IntelliSense support.
// 1.0 | 2026-04-14 | Nitish Singh | Initial implementation of core casing and reversal tests.
//--------------------------------------------------------------------------------------------------

using System.Threading.Tasks;
using Xunit;
using Microsoft.AspNetCore.Mvc.Testing;

namespace CaseConversion.Tests.Integration;

/// <summary>
/// Integration test suite for fundamental Word Case REST API conversions.
/// Validates core string transformation logic backed by the native C++ engine.
/// </summary>
/// <remarks>
/// Inherits ApiTestBase for shared HTTP client configuration. Ensures ASCII casing 
/// behavior remains consistent across the P/Invoke boundary for "Happy Path" scenarios.
/// </remarks>
public class BasicConversionTests : ApiTestBase
{
    public BasicConversionTests(WebApplicationFactory<Program> factory)
        : base(factory)
    {
    }

    //===================================================================
    // Standard Case Transformations
    //===================================================================

    [Fact]
    [Trait("Category", "StandardCasing")]
    public async Task Convert_LowerCase_ReturnsAllSmallCharacters()
        => Assert.Equal("hello", await ConvertAsync("HELLO", 3));

    [Fact]
    [Trait("Category", "StandardCasing")]
    public async Task Convert_UpperCase_ReturnsAllCapitalCharacters()
        => Assert.Equal("HELLO", await ConvertAsync("hello", 4));

    [Fact]
    [Trait("Category", "StandardCasing")]
    public async Task Convert_CapitalizeWords_ReturnsTitleCase()
        => Assert.Equal("Hello World", await ConvertAsync("hello world", 2));

    [Fact]
    [Trait("Category", "StandardCasing")]
    public async Task Convert_SentenceCase_ReturnsInitialCapitalOnly()
        => Assert.Equal("Hello world", await ConvertAsync("hELLO WORLD", 5));

    //===================================================================
    // Complex Casing & Ordering
    //===================================================================

    [Fact]
    [Trait("Category", "PatternCasing")]
    public async Task Convert_ToggleCase_InvertsCharacterCasing()
        => Assert.Equal("hEllO", await ConvertAsync("HeLLo", 6));

    [Fact]
    [Trait("Category", "PatternCasing")]
    public async Task Convert_AlternatingCase_ReturnsStaggeredCasing()
        => Assert.Equal("HeLlO WoRlD", await ConvertAsync("hello world", 1));

    [Fact]
    [Trait("Category", "Structural")]
    public async Task Convert_Reverse_ReturnsMirroredString()
        => Assert.Equal("!dlroW olleH", await ConvertAsync("Hello World!", 7));
}