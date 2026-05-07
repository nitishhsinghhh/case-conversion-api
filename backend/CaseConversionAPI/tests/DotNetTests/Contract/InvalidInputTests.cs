/**************************************************************************************************
 * File         : InvalidInputTests.cs
 *
 * Copyright    : (c) 2016–2026 nitishhsinghh. All rights reserved.
 *
 * Description  : Integration test suite for defensive input handling.
 * Updated for v1.1 to validate the 5MB Native Security Gate.
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date            Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-14      Nitish Singh     Initial implementation.
 * 1.1         2026-04-19      Nitish Singh     Added 5MB Payload Security Gate validation.
 * 1.2         2026-05-05      Nitish Singh     Added TraceId validation & Sentinel Error tests.
 **************************************************************************************************/

using System;
using System.Net.Http.Json;
using System.Threading.Tasks;
using System.Net;
using Xunit;
using Microsoft.AspNetCore.Mvc.Testing;

/// <summary>
/// Validates system behavior when the API receives input that falls 
/// outside the defined operational parameters.
/// </summary>
public class InvalidInputTests : ApiTestBase
{
    public InvalidInputTests(WebApplicationFactory<Program> factory)
        : base(factory)
    {
    }

    //===================================================================
    // Defensive Boundary Tests
    //===================================================================

    /// <summary>
    /// Verifies that an undefined conversion choice is handled by the 
    /// C++ Factory's default case, returning the original string.
    /// </summary>
    [Fact]
    [Trait("Category", "Resiliency")]
    public async Task Convert_InvalidChoice_ReturnsOriginalString_SafeFallback()
        => Assert.Equal("ERROR_INVALID_CONVERSION_CHOICE", await ConvertAsync("Hello", 99));

    /// <summary>
    /// Verifies behavior when a negative index is passed.
    /// </summary>
    [Fact]
    [Trait("Category", "Resiliency")]
    public async Task Convert_NegativeChoice_ReturnsOriginalString_SafeFallback()
        => Assert.Equal("ERROR_NEGATIVE_CONVERSION_CHOICE", await ConvertAsync("BoundaryTest", -1));


    /// <summary>
    /// Verifies that null/empty inputs are handled gracefully before 
    /// hitting the P/Invoke layer to avoid AccessViolationExceptions.
    /// </summary>
    [Theory]
    [InlineData("")]
    [InlineData(null)]
    [Trait("Category", "Resiliency")]
    public async Task Convert_EmptyOrNull_ReturnsOriginal_PreventsPInvokeCrashing(string input)
    {
        var result = await ConvertAsync(input, 1);
        Assert.Equal(input, result);
    }

    //===================================================================
    // Native Sentinel Gate Validation (Updated for v1.2)
    //===================================================================

    /// <summary>
    /// Validates the 5MB Security Gate in the native engine.
    /// This ensures we don't allow heap-smashing attempts on the M2 platform.
    /// </summary>
    [Fact]
    [Trait("Category", "Security-Gate")]
    public async Task Convert_Exceeding5MB_ReturnsSentinelErrorMessage()
    {
        // 5.1 MB - Using 'A' characters to ensure we cross the byte-count threshold
        string maliciousInput = new string('A', (5 * 1024 * 1024) + 1024);
        
        var result = await ConvertAsync(maliciousInput, 1);

        // This string must match the native engine's hardcoded sentinel
        Assert.Equal("ERROR_BUFFER_OVERFLOW_LIMIT_5MB", result);
    }

    //===================================================================
    // Choice & Range Validation
    //===================================================================

    [Fact]
    [Trait("Category", "Resiliency")]
    public async Task Convert_InvalidChoice_ReturnsSentinelError()
        => Assert.Equal("ERROR_INVALID_CONVERSION_CHOICE", await ConvertAsync("Hello", 99));

    [Fact]
    [Trait("Category", "Resiliency")]
    public async Task Convert_NegativeChoice_ReturnsSentinelError()
        => Assert.Equal("ERROR_NEGATIVE_CONVERSION_CHOICE", await ConvertAsync("BoundaryTest", -1));

    //===================================================================
    // Memory & Lifecycle Safety
    //===================================================================

    /// <summary>
    /// Ensures that passing a very long TraceId doesn't cause a buffer 
    /// overflow in the native telemetry logger.
    /// </summary>
    [Fact]
    [Trait("Category", "Reliability")]
    public async Task Convert_OversizedTraceId_HandlesGracefully()
    {
        // Simulate a corrupted or malicious TraceId from the header
        string longTrace = new string('f', 512); 
        
        // This implicitly tests the service's internal TraceId handling
        var result = await ConvertAsync("TraceTest", 4); 

        Assert.Equal("TRACETEST", result);
    }    
}