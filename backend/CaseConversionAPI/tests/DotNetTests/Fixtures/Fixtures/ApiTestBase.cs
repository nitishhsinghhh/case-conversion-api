/**************************************************************************************************
 * File        : ApiTestBase.cs
 *
 * Copyright   : (c) 2016–2026 nitishhsinghh. All rights reserved.
 *               This material may be reproduced for teaching and learning purposes only.
 *               It is not to be used in industry or for commercial purposes.
 *
 * Class       : ApiTestBase
 *
 * Description : Base infrastructure for Word Case API integration testing.
 *               Provides an in-memory HTTP execution environment to validate
 *               the complete request-to-native-processing pipeline.
 *
 * Notes       : - Implements IClassFixture for efficient reuse of WebApplicationFactory.
 *               - Centralizes HTTP communication, JSON marshalling, and validation logic.
 *               - Ensures consistent API contract verification across all test suites.
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date        Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-14  Nitish Singh    Initial implementation of API test orchestration layer
 * 1.1         2026-05-06  Nitish Singh    Added explicit UTF-8 byte length calculation for v1.5 
 * engine
 **************************************************************************************************/

using System.Net.Http;
using System.Net.Http.Json;
using System.Text;
using System.Threading.Tasks;
using Xunit;
using Microsoft.AspNetCore.Mvc.Testing;
using StringConversionAPI.Models;

/// <summary>
/// Provides a shared execution context for all API integration tests.
/// Manages the lifecycle of the in-memory TestServer and provides
/// high-level abstraction for REST interactions.
/// </summary>
public abstract class ApiTestBase : IClassFixture<WebApplicationFactory<Program>>
{
    //===================================================================
    // Infrastructure Members
    //===================================================================

    /// <summary>
    /// Virtualized HTTP Client used to dispatch requests to the 
    /// in-memory WebApplicationFactory host.
    /// </summary>
    protected readonly HttpClient Client;

    /// <summary>
    /// Initializes a new instance of the test base with a shared 
    /// application factory.
    /// </summary>
    /// <param name="factory">The virtual host factory for the ASP.NET Core Program.</param>
    protected ApiTestBase(WebApplicationFactory<Program> factory)
    {
        // Configure the client with a base address and handle cookies/redirects
        Client = factory.CreateClient();
    }

    //===================================================================
    // API Orchestration Helpers
    //===================================================================

    /// <summary>
    /// Encapsulates the POST request logic to the conversion endpoint.
    /// Handles JSON serialization, transport, and deserialization of the result.
    /// </summary>
    /// <param name="text">The raw input string to be converted.</param>
    /// <param name="choice">The integer ID of the native conversion strategy.</param>
    /// <param name="length">The physical byte length of the input string for UTF-8 encoding.</param>
    /// <returns>The processed string returned by the native C++ engine.</returns>
    /// <exception cref="HttpRequestException">Thrown if the API returns a non-200 status code.</exception>
    protected async Task<string> ConvertAsync(string text, int choice)
    {
        int length = string.IsNullOrEmpty(text) ? 0 : Encoding.UTF8.GetByteCount(text);

        // 1. Dispatch POST request
        var response = await Client.PostAsJsonAsync(
            "/api/WordCase/convert",
            new { text, choice, length });

        // 2. Safely read raw content to avoid stream-parsing crashes
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
                
            // If the native engine flagged an error but returned a 200, 
            // it might have put the error in a different field or left Output empty.
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
            // Fallback: If it's not valid JSON but returned 200, return raw string
            return rawContent;
        }
    }
}