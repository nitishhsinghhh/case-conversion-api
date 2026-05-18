/**************************************************************************************************
 * File         : WordCaseController.cs
 *
 * Copyright    : (c) 2016–2026 nitishhsinghh. All rights reserved.
 * This material may be reproduced for teaching and learning purposes only.
 * It is not to be used in industry or for commercial purposes.
 *
 * Class        : WordCaseController
 *
 * Description  : REST API controller exposing string case conversion operations.
 * Delegates processing to the service layer, which interfaces with
 * the native C++ conversion engine.
 *
 * Notes        : - Implements HTTP POST endpoints for single and parallel batch conversion.
 * - Uses dependency injection for ProcessStringService.
 * - Optimized for Apple M2 P-Core saturation via Parallel.ForEachAsync.
 * - Validates input against the 5MB Security Gate defined in the C++ layer.
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-11     Nitish Singh     Initial implementation of API controller.
 * 1.1         2026-04-19     Nitish Singh     Added Parallel Batch Endpoint (4-Thread M2 Support).
 * Integrated async/await for hardware-bound tasks.
 * 1.2         2026-04-20     Nitish Singh     Final consolidation: Resolved CS0111 (Duplicate members),
 * fixed CS0117, and removed CS0162 unreachable code.
 **************************************************************************************************/

using Microsoft.AspNetCore.Mvc;
using StringConversionAPI.Models;
using StringConversionAPI.Services;
using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Linq;
using Microsoft.AspNetCore.Authorization;

namespace StringConversionAPI.Controllers
{
    public record LoginRequest(string Username, string Password);

    /// <summary>
    /// Model for Parallel Batch Requests.
    /// </summary>
    public class BatchRequest
    {
        public IEnumerable<string> Texts { get; set; } = new List<string>();
        public int Choice { get; set; }
    }
    [ApiController]
    [Route("api/[controller]")]
    public class AuthController : ControllerBase
    {
        private readonly ITokenService _tokenService;

        public AuthController(ITokenService tokenService) 
            => _tokenService = tokenService;

        [HttpPost("login")]
        public IActionResult Login([FromBody] LoginRequest request)
        {
            if (request.Username == "admin" && request.Password == "password")
            {
                var token = _tokenService.GenerateToken(request.Username, new[] { "Admin", "User" });
                return Ok(new { Token = token });
            }
            return Unauthorized(new { Message = "Invalid credentials" });
        }
    }

    [ApiController]
    [Route("api/[controller]")]
    [AllowAnonymous]
    public class WordCaseController : ControllerBase 
    {
        private readonly ProcessStringService _service;

        public WordCaseController(ProcessStringService service)
        {
            _service = service;
        }

        /// <summary>
        /// Single-string conversion endpoint (Sequential).
        /// </summary>
        [HttpPost("convert")]
        public IActionResult Convert([FromBody] ConvertRequest request)
        {
            // Basic null check for the JSON object itself
            if (request == null)
                return BadRequest("Payload is null.");

            try 
            {
                // Allow empty strings through (satisfied by previous fix)
                if (request.Text == null) 
                    return Ok(new ConvertResponse { ConvertedText = null });
                
                if (request.Text == string.Empty)
                    return Ok(new ConvertResponse { ConvertedText = string.Empty });

                // Process via service layer (Lock 2: 5MB Native Guard)
                var result = _service.Convert(request.Text, request.Choice);
                
                // THE CRITICAL GATE:
                // Use String.Equals with Ordinal comparison to ensure the sentinel 
                // from the C++ layer is matched exactly.
                if (string.Equals(result, "ERROR_BUFFER_OVERFLOW_LIMIT_5MB", StringComparison.Ordinal))
                {
                    return Ok(new ConvertResponse { ConvertedText = "ERROR_BUFFER_OVERFLOW_LIMIT_5MB" });
                }

                // FIXED: Property name ConvertedText matches v1.4 Models
                return Ok(new ConvertResponse 
                {   Input = request.Text,
                    Choice = request.Choice,
                    ConvertedText = result ?? string.Empty
                });
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"Unexpected error: {ex}");
                return StatusCode(500, "An unexpected error occurred.");
            }
        }
    
        /// <summary>
        /// M2-Optimized Parallel Endpoint.
        /// Targets 4 Performance Cores for high-throughput batch processing.
        /// </summary>
        [HttpPost("convert-batch")]
        public async Task<IActionResult> ConvertBatchAsync([FromBody] BatchRequest request)
        {
            if (request == null || request.Texts == null) 
                return BadRequest("Payload or inputs are null.");

            try
            {
                // Call the service - let IT handle the 20MB and 4-core logic
                var results = await _service.ConvertBatchAsync(request.Texts, request.Choice);
                
                // Return the batch results
                return Ok(results);
            }
            catch (ArgumentException ex)
            {
                // This catches the 20MB limit from the service
                return BadRequest(ex.Message);
            }
            catch (Exception)
            {
                return StatusCode(500, "Internal batch processing error.");
            }
        }
    }
}