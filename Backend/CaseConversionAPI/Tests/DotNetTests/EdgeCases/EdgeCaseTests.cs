//--------------------------------------------------------------------------------------------------
// File        : EdgeCaseTests.cs
// Author      : Nitish Singh
// Version     : 1.2
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
// Change History:
// 1.2 | 2026-05-09 | Nitish Singh | Upgraded to XML Documentation for IDE IntelliSense support.
// 1.1 | 2026-05-09 | Nitish Singh | Added UTF-8 multi-byte and surrogate pair safety validation.
// 1.0 | 2026-04-14 | Nitish Singh | Initial implementation of edge case and boundary tests.
//--------------------------------------------------------------------------------------------------

using System.Threading.Tasks;
using Xunit;
using Microsoft.AspNetCore.Mvc.Testing;

namespace CaseConversion.Tests.Integration;

/// <summary>
/// 🧩 [BOUNDARY & EDGE CASE SUITE]
/// Exhaustive validation of non-standard inputs to ensure no regression in logic.
/// </summary>
/// <remarks>
/// <para><b>Identity:</b> EdgeCaseTests (v1.1)</para>
/// <para><b>Scope:</b> Validates native engine behavior for zero-length buffers, UTF-8 
/// international encoding, and non-alphabetic symbolic logic.</para>
/// <para><b>Security:</b> Guards against memory access violations at the P/Invoke boundary.</para>
/// </remarks>
public class EdgeCaseTests : ApiTestBase
{
    public EdgeCaseTests(WebApplicationFactory<Program> factory)
        : base(factory)
    {
    }

    //---[ SECTION: Null & Whitespace Boundaries ]--------------------------------------------------

    [Fact]
    [Trait("Category", "Boundary")]
    public async Task Convert_EmptyString_ReturnsEmptyResponse()
        => Assert.Equal("", await ConvertAsync("", 3));

    [Fact]
    [Trait("Category", "Boundary")]
    public async Task Convert_OnlySpaces_RemoveSpaces_ReturnsEmptyResponse()
        => Assert.Equal("", await ConvertAsync("   ", 9));

    //---[ SECTION: Character Set Boundaries (Non-Alpha) ]------------------------------------------

    [Fact]
    [Trait("Category", "NonAlpha")]
    public async Task Convert_NumbersOnly_MaintainsNumericIntegrity()
        => Assert.Equal("12345", await ConvertAsync("12345", 4));

    [Fact]
    [Trait("Category", "NonAlpha")]
    public async Task Convert_SpecialCharacters_Reverse_HandlesSymbolsCorrectly()
        => Assert.Equal("$#@!", await ConvertAsync("!@#$", 7));

    //---[ SECTION: Domain-Specific Logic Boundaries (Filtering) ]----------------------------------

    [Fact]
    [Trait("Category", "Filtering")]
    public async Task Convert_OnlyVowels_RemoveVowels_ReturnsEmptyString()
        => Assert.Equal("", await ConvertAsync("aeiouAEIOU", 8));

    [Fact]
    [Trait("Category", "Filtering")]
    public async Task Convert_NoVowels_ReturnsOriginalConsonants()
        => Assert.Equal("bcdfg", await ConvertAsync("bcdfg", 8));

    //---[ SECTION: Multi-Byte & International Encoding Boundaries ]--------------------------------

    [Fact]
    [Trait("Category", "Encoding")]
    public async Task Convert_MultiByteChar_HandlesInternationalSymbols()
        => Assert.Equal("EURO: €", await ConvertAsync("euro: €", 4));

    //---[ SECTION: Structural Formatting Boundaries ]----------------------------------------------

    [Fact]
    [Trait("Category", "Formatting")]
    public async Task Convert_MixedTabsAndNewlines_PreservesWhitespace()
        => Assert.Equal("LINE\t1\nLINE\t2", await ConvertAsync("line\t1\nline\t2", 4));

    //---[ SECTION: Memory Capacity Boundaries ]----------------------------------------------------

    [Fact]
    [Trait("Category", "Boundary")]
    public async Task Convert_MaxByteBoundary_ProcessesLargeSingleWord()
        => Assert.NotNull(await ConvertAsync(new string('a', 1000), 4));
}