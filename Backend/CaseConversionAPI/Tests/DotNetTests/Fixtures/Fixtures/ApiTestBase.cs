//--------------------------------------------------------------------------------------------------
// File        : ApiTestBase.cs
// Author      : Nitish Singh
// Version     : 1.2
// License     : Apache License, Version 2.0 (See below for restrictions)
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
// Change History (Reverse Chronological):
// 1.2 | 2026-05-09 | Nitish Singh | Upgraded to XML Documentation for IDE IntelliSense support.
// 1.1 | 2026-05-09 | Nitish Singh | Enhanced marshaling and Sentinel error handling for v1.5 core.
// 1.0 | 2026-04-14 | Nitish Singh | Initial implementation of API test orchestration layer.
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
/// [TEST INFRASTRUCTURE BASE]
/// Provides a shared execution context for all API integration tests.
/// </summary>
/// <remarks>
/// <para><b>Identity:</b> ApiTestBase (v1.2)</para>
/// <para><b>Responsibility:</b> Manages the lifecycle of the in-memory TestServer and provides 
/// high-level abstraction for REST interactions with the native-managed bridge.</para>
/// <para><b>Features:</b> Centralized JSON marshaling, UTF-8 byte calculation, and sentinel 
/// error detection logic.</para>
/// </remarks>
public abstract class ApiTestBase : IClassFixture<WebApplicationFactory<Program>>
{
    //---[ SECTION: Infrastructure Members ]--------------------------------------------------------

    /// <summary>
    /// Virtualized HTTP Client used to dispatch requests to the 
    /// in-memory WebApplicationFactory host.
    /// </summary>
    protected readonly HttpClient Client;

    /// <summary>
    /// Initializes the test base with a shared application factory.
    /// </summary>
    /// <param name="factory">The virtual host factory for the ASP.NET Core Program.</param>
    protected ApiTestBase(WebApplicationFactory<Program> factory)
    {
        Client = factory.CreateClient();
    }

    //---[ SECTION: API Orchestration Helpers ]-----------------------------------------------------

    /// <summary>
    /// Encapsulates the POST request logic to the conversion endpoint.
    /// Handles JSON serialization, transport, and defensive deserialization.
    /// </summary>
    /// <param name="text">The raw input string to be converted.</param>
    /// <param name="choice">The integer ID of the native conversion strategy.</param>
    /// <returns>The processed string returned by the native C++ engine or a sentinel error.</returns>
    protected async Task<string> ConvertAsync(string text, int choice)
    {
        // Calculate physical byte count for UTF-8 (Required for Native v1.5 memory allocation)
        int length = string.IsNullOrEmpty(text) ? 0 : Encoding.UTF8.GetByteCount(text);

        // 1. Dispatch POST request
        var response = await Client.PostAsJsonAsync(
            "/api/WordCase/convert",
            new { text, choice, length });

        // 2. Safely read raw content to avoid stream-parsing crashes during security triggers
        var bytes = await response.Content.ReadAsByteArrayAsync();
        var rawContent = Encoding.UTF8.GetString(bytes);

        // 3. Handle Sentinel/Security Triggers (Non-200 Responses)
        if (!response.IsSuccessStatusCode)
        {
            return !string.IsNullOrWhiteSpace(rawContent) 
                ? rawContent 
                : $"Error: {response.StatusCode}";
        }

        // 4. Handle Success Path - Check for empty body before JSON parsing
        if (string.IsNullOrWhiteSpace(rawContent))
        {
            return text == null ? null : string.Empty;
        }

        try 
        {
            var options = new System.Text.Json.JsonSerializerOptions { PropertyNameCaseInsensitive = true };
            var result = System.Text.Json.JsonSerializer.Deserialize<ConvertResponse>(rawContent, options);
                
            if (result == null)
            {
                return rawContent; 
            }

            if (text == null) 
                return null;

            return result.ConvertedText ?? result.Output ?? string.Empty;
        }
        catch (System.Text.Json.JsonException)
        {
            // Fallback: If it's not valid JSON but returned 200, return raw string (Sentinel behavior)
            return rawContent;
        }
    }
}