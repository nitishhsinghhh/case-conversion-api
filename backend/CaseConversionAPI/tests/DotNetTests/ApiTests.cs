//--------------------------------------------------------------------------------------------------
// File        : ApiTests.cs
// Author      : Nitish Singh
// Version     : 1.4
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
// RESTRICTION: This material may be reproduced for teaching and learning purposes only. 
// It is not to be used in industry or for commercial purposes.
//
// Change History:
// 1.4 | 2026-05-09 | Nitish Singh | Upgraded to XML Documentation for IDE IntelliSense support.
// 1.3 | 2026-05-06 | Nitish Singh | Added explicit length calculation for Native v1.5 interop.
// 1.2 | 2026-04-14 | Nitish Singh | Synced with C++ test expectations and API contract.
// 1.0 | 2026-04-14 | Nitish Singh | Initial implementation of API integration tests.
//--------------------------------------------------------------------------------------------------

using System.Net.Http;
using System.Net.Http.Json;
using System.Text;
using System.Threading.Tasks;
using Xunit;
using Microsoft.AspNetCore.Mvc.Testing;
using StringConversionAPI.Models;

namespace CaseConversion.Tests.Integration;

/// <summary>
/// [FULL-STACK INTEGRATION SUITE]
/// Comprehensive integration test suite for the Word Case REST API.
/// </summary>
/// <remarks>
/// <para><b>Identity:</b> ApiTests (v1.4)</para>
/// <para><b>Purpose:</b> Mirrors the C++ test suite to ensure cross-layer consistency between 
/// the .NET API gateway and the native C++ string processing engine.</para>
/// <para><b>Technical Note:</b> Handles UTF-8 byte-count calculations to ensure 0-copy 
/// compatibility with Native v1.5.* memory management.</para>
/// </remarks>
public class ApiTests : IClassFixture<WebApplicationFactory<Program>>
{
    private readonly HttpClient _client;

    public ApiTests(WebApplicationFactory<Program> factory)
    {
        _client = factory.CreateClient();
    }

    /// <summary>
    /// Helper method to simulate a full API request/response cycle.
    /// </summary>
    private async Task<string> ConvertAsync(string text, int choice)
    {
        // Calculate physical byte count for UTF-8 encoding (Critical for Native Buffer Allocation)
        int length = string.IsNullOrEmpty(text) ? 0 : Encoding.UTF8.GetByteCount(text);

        var response = await _client.PostAsJsonAsync(
            "/api/WordCase/convert",
            new { 
                text, 
                choice,
                length // Explicit length passed to native-managed bridge
            });

        response.EnsureSuccessStatusCode();

        var result = await response.Content.ReadFromJsonAsync<ConvertResponse>();
        Assert.NotNull(result);

        return result.Output;
    }

    //---[ SECTION: 1. BASIC CONVERSIONS (MATCH C++ TESTS) ]----------------------------------------

    [Fact] public async Task Convert_LowerCase() 
        => Assert.Equal("hello", await ConvertAsync("HELLO", 3));

    [Fact] public async Task Convert_UpperCase() 
        => Assert.Equal("HELLO", await ConvertAsync("hello", 4));

    [Fact] public async Task Convert_CapitalizeWords() 
        => Assert.Equal("Hello World", await ConvertAsync("hello world", 2));

    [Fact] public async Task Convert_SentenceCase() 
        => Assert.Equal("Hello world", await ConvertAsync("hELLO WORLD", 5));

    [Fact] public async Task Convert_ToggleCase() 
        => Assert.Equal("hEllO", await ConvertAsync("HeLLo", 6));

    [Fact] public async Task Convert_AlternatingCase() 
        => Assert.Equal("HeLlO WoRlD", await ConvertAsync("hello world", 1));

    [Fact] public async Task Convert_Reverse() 
        => Assert.Equal("!dlroW olleH", await ConvertAsync("Hello World!", 7));

    //---[ SECTION: 2. ADVANCED CONVERSIONS (MATCH C++ TESTS) ]-------------------------------------

    [Fact] public async Task Convert_InvertWords() 
        => Assert.Equal("olleH dlroW", await ConvertAsync("Hello World", 10));

    [Fact] public async Task Convert_SnakeCase() 
        => Assert.Equal("hello_world_example", await ConvertAsync("Hello World Example", 11));

    [Fact] public async Task Convert_KebabCase() 
        => Assert.Equal("hello-world-example", await ConvertAsync("Hello World Example", 12));

    [Fact] public async Task Convert_RemoveSpaces() 
        => Assert.Equal("HelloWorld", await ConvertAsync("Hello World", 9));

    [Fact] public async Task Convert_RemoveVowels() 
        => Assert.Equal("Hll Wrld", await ConvertAsync("Hello World", 8));

    [Fact] public async Task Convert_LeetSpeak() 
        => Assert.Equal("7357", await ConvertAsync("Test", 13));

    //---[ SECTION: 3. EDGE CASES (MATCH C++ TESTS) ]-----------------------------------------------

    [Fact] public async Task Convert_EmptyString() 
        => Assert.Equal("", await ConvertAsync("", 3));

    [Fact] public async Task Convert_OnlySpaces_RemoveSpaces() 
        => Assert.Equal("", await ConvertAsync("   ", 9));

    [Fact] public async Task Convert_NumbersOnly() 
        => Assert.Equal("12345", await ConvertAsync("12345", 4));

    [Fact] public async Task Convert_SpecialCharacters_Reverse() 
        => Assert.Equal("$#@!", await ConvertAsync("!@#$", 7));

    [Fact] public async Task Convert_OnlyVowels_RemoveVowels() 
        => Assert.Equal("", await ConvertAsync("aeiouAEIOU", 8));

    [Fact] public async Task Convert_NoVowels() 
        => Assert.Equal("bcdfg", await ConvertAsync("bcdfg", 8));

    //---[ SECTION: 4. INVALID INPUT (MATCH API CONTRACT) ]-----------------------------------------

    /// <summary>
    /// Validates that invalid choices return the specific sentinel error string 
    /// rather than defaulting to lower-case (differing from CLI behavior).
    /// </summary>
    [Fact]
    public async Task Convert_InvalidChoice_ReturnsSentinelError()
    {
        var output = await ConvertAsync("Hello", 99);
        Assert.Equal("ERROR_INVALID_CONVERSION_CHOICE", output);
    }
}