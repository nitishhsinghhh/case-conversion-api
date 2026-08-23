/**************************************************************************************************
 * File         : WordCaseController.cs
 *
 * Copyright    : (c) 2016–2026 Nitish Singh. All rights reserved.
 * License      : Licensed under the Apache License, Version 2.0 (the "License");
 *                you may not use this file except in compliance with the License.
 *                You may obtain a copy of the License at
 *
 *                http://www.apache.org/licenses/LICENSE-2.0
 *
 *                Unless required by applicable law or agreed to in writing, software
 *                distributed under the License is distributed on an "AS IS" BASIS,
 *                WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *                See the License for the specific language governing permissions and
 *                limitations under the License.
 *
 * Description  : REST API controllers exposing authentication and high-performance string
 *                case conversion operations. Integrates unmanaged execution architectures
 *                with managed .NET infrastructure components.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-11     Nitish Singh     Initial implementation of web API controllers.
 * 1.1         2026-04-19     Nitish Singh     Engineered parallel batch endpoint utilizing async
 *                                             orchestration designed for Apple M2 core topologies.
 * 1.2         2026-04-20     Nitish Singh     Consolidated routing structures, resolved compilation
 *                                             failures CS0111 and CS0117, and pruned dead execution
 *                                             branches.
 * 1.3         2026-08-23     Nitish Singh     Unified native-engine resolution for C++ and Rust
 *                                             implementations, added public engine aliases,
 *                                             standardized validation, and hardened controller
 *                                             dependency handling.
 **************************************************************************************************/

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using StringConversionAPI.Models;
using StringConversionAPI.Services;

namespace StringConversionAPI.Controllers
{
    /// <summary>
    /// Represents the request payload utilized during identity validation.
    /// </summary>
    /// <param name="Username">The identity username claim string value.</param>
    /// <param name="Password">The plain-text authentication matching credential.</param>
    public sealed record LoginRequest(string Username, string Password);

    /// <summary>
    /// Defines data parameters expected for incoming parallel array processing requests.
    /// </summary>
    public sealed class BatchRequest
    {
        /// <summary>
        /// Gets or sets the collection of individual text items to process concurrently.
        /// </summary>
        public IEnumerable<string> Texts { get; set; } = new List<string>();

        /// <summary>
        /// Gets or sets the transformation routine index matching unmanaged engine structures.
        /// </summary>
        public int Choice { get; set; }

        /// <summary>
        /// Gets or sets the optional native engine identifier.
        /// Supported values are <c>cpp</c> and <c>rust</c>.
        /// </summary>
        public string? EngineType { get; set; }
    }

    /// <summary>
    /// Exposes authentication routes used to provision tokens verifying outbound client identities.
    /// </summary>
    [ApiController]
    [Route("api/[controller]")]
    [Produces("application/json")]
    public sealed class AuthController : ControllerBase
    {
        private readonly ITokenService _tokenService;

        /// <summary>
        /// Initializes a new instance of the <see cref="AuthController"/> class.
        /// </summary>
        /// <param name="tokenService">The identity token handling service instance.</param>
        public AuthController(ITokenService tokenService)
        {
            _tokenService = tokenService
                ?? throw new ArgumentNullException(nameof(tokenService));
        }

        /// <summary>
        /// Validates authorization metadata and generates an identity context bearer string.
        /// </summary>
        /// <param name="request">The target identity request payload.</param>
        /// <returns>An action result containing a validated token block or unauthorized markers.</returns>
        [HttpPost("login")]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status400BadRequest)]
        [ProducesResponseType(StatusCodes.Status401Unauthorized)]
        public IActionResult Login([FromBody] LoginRequest request)
        {
            if (request == null)
            {
                return BadRequest(new
                {
                    Message = "The login payload structure cannot be parsed as a valid model."
                });
            }

            // Standard credential screening setup for baseline operational testing.
            if (request.Username == "admin" &&
                request.Password == "password")
            {
                string token = _tokenService.GenerateToken(
                    request.Username,
                    new[] { "Admin", "User" });

                return Ok(new { Token = token });
            }

            return Unauthorized(new
            {
                Message = "Invalid authentication credentials supplied."
            });
        }
    }

    /// <summary>
    /// Exposes entry boundaries for handling unmanaged string mutation pipelines.
    /// </summary>
    [ApiController]
    [Route("api/[controller]")]
    [AllowAnonymous]
    [Produces("application/json")]
    public sealed class WordCaseController : ControllerBase
    {
        private readonly IEnumerable<INativeStringEngine> _engines;

        /// <summary>
        /// Initializes a new instance of the <see cref="WordCaseController"/> class.
        /// </summary>
        /// <param name="engines">
        /// All registered native string processing engines.
        /// </param>
        public WordCaseController(IEnumerable<INativeStringEngine> engines)
        {
            _engines = engines
                ?? throw new ArgumentNullException(nameof(engines));
        }

        /// <summary>
        /// Resolves the requested native engine.
        ///
        /// Public identifiers:
        ///     cpp  -> CppEngine
        ///     rust -> RustEngine
        ///
        /// Registered implementation names are also accepted.
        ///
        /// If no engine is specified, C++ is selected for backward compatibility.
        /// </summary>
        /// <param name="engineType">
        /// Requested engine identifier.
        /// </param>
        /// <returns>
        /// The resolved native string engine, or <c>null</c> when unavailable.
        /// </returns>
        private INativeStringEngine? ResolveEngine(string? engineType)
        {
            string requestedEngine = string.IsNullOrWhiteSpace(engineType)
                ? "cpp"
                : engineType.Trim();

            string normalizedEngine = requestedEngine.ToLowerInvariant() switch
            {
                "cpp" => "CppEngine",
                "cppengine" => "CppEngine",

                "rust" => "RustEngine",
                "rustengine" => "RustEngine",

                _ => requestedEngine
            };

            return _engines.FirstOrDefault(engine =>
                engine.Name.Equals(
                    normalizedEngine,
                    StringComparison.OrdinalIgnoreCase));
        }

        /// <summary>
        /// Transforms a single input sequence through the selected native engine.
        /// </summary>
        /// <param name="request">
        /// The request containing text, conversion choice and optional engine.
        /// </param>
        /// <returns>A structured response carrying the converted payload.</returns>
        [HttpPost("convert")]
        [ProducesResponseType(typeof(ConvertResponse), StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status400BadRequest)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public IActionResult Convert([FromBody] ConvertRequest request)
        {
            if (request == null)
            {
                return BadRequest(
                    "The incoming conversion request structural instance cannot be null.");
            }

            string requestedEngine = string.IsNullOrWhiteSpace(request.EngineType)
                ? "cpp"
                : request.EngineType.Trim();

            INativeStringEngine? engine =
                ResolveEngine(request.EngineType);

            if (engine == null)
            {
                return BadRequest(
                    $"Engine '{requestedEngine}' not found.");
            }

            try
            {
                if (request.Text == null)
                {
                    return Ok(new ConvertResponse
                    {
                        Input = null,
                        Choice = request.Choice,
                        ConvertedText = null!
                    });
                }

                if (request.Text.Length == 0)
                {
                    return Ok(new ConvertResponse
                    {
                        Input = request.Text,
                        Choice = request.Choice,
                        ConvertedText = string.Empty
                    });
                }

                string result = engine.Convert(
                    request.Text,
                    request.Choice);

                // Preserve the native security sentinel.
                if (string.Equals(
                        result,
                        "ERROR_BUFFER_OVERFLOW_LIMIT_5MB",
                        StringComparison.Ordinal))
                {
                    return Ok(new ConvertResponse
                    {
                        Input = request.Text,
                        Choice = request.Choice,
                        ConvertedText = "ERROR_BUFFER_OVERFLOW_LIMIT_5MB"
                    });
                }

                return Ok(new ConvertResponse
                {
                    Input = request.Text,
                    Choice = request.Choice,
                    ConvertedText = result ?? string.Empty
                });
            }
            catch (Exception ex)
            {
                Debug.WriteLine(
                    $"Unexpected conversion pipeline runtime error occurred: {ex}");

                return StatusCode(
                    StatusCodes.Status500InternalServerError,
                    "An unexpected anomaly interrupted core conversion processing steps.");
            }
        }

        /// <summary>
        /// Orchestrates concurrent conversions for an array of payloads.
        /// </summary>
        /// <param name="request">
        /// The request containing multiple text items, conversion choice
        /// and optional engine.
        /// </param>
        /// <returns>An ordered collection containing processed transformations.</returns>
        [HttpPost("convert-batch")]
        [ProducesResponseType(typeof(IEnumerable<string>), StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status400BadRequest)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public async Task<IActionResult> ConvertBatchAsync(
            [FromBody] BatchRequest request)
        {
            if (request == null || request.Texts == null)
            {
                return BadRequest(
                    "The batch conversion structural payload or input sequence context cannot be null values.");
            }

            string requestedEngine = string.IsNullOrWhiteSpace(request.EngineType)
                ? "cpp"
                : request.EngineType.Trim();

            INativeStringEngine? engine =
                ResolveEngine(request.EngineType);

            if (engine == null)
            {
                return BadRequest(
                    $"Engine '{requestedEngine}' not found.");
            }

            try
            {
                IEnumerable<string> results =
                    await engine.ConvertBatchAsync(
                        request.Texts,
                        request.Choice);

                return Ok(results);
            }
            catch (ArgumentException ex)
            {
                // Preserve payload/security validation failures.
                return BadRequest(ex.Message);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(
                    $"Unexpected parallel engine batch anomaly intercepted: {ex}");

                return StatusCode(
                    StatusCodes.Status500InternalServerError,
                    "Internal parallel pipeline task orchestration error.");
            }
        }
    }

    /// <summary>
    /// Provides native-engine performance comparison capabilities.
    /// </summary>
    [ApiController]
    [Route("api/benchmark")]
    public sealed class BenchmarkController : ControllerBase
    {
        private readonly IEnumerable<INativeStringEngine> _engines;

        /// <summary>
        /// Initializes a new instance of the <see cref="BenchmarkController"/> class.
        /// </summary>
        /// <param name="engines">All registered native string processing engines.</param>
        public BenchmarkController(IEnumerable<INativeStringEngine> engines)
        {
            _engines = engines
                ?? throw new ArgumentNullException(nameof(engines));
        }

        /// <summary>
        /// Compares the average conversion latency of all registered native engines.
        /// </summary>
        /// <param name="input">Input text used for benchmarking.</param>
        /// <param name="choice">Conversion operation identifier.</param>
        /// <returns>Average conversion latency in milliseconds per engine.</returns>
        [HttpPost("compare")]
        [ProducesResponseType(typeof(Dictionary<string, double>), StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status400BadRequest)]
        public IActionResult Compare(
            [FromBody] string input,
            [FromQuery] int choice)
        {
            if (input == null)
            {
                return BadRequest("Benchmark input cannot be null.");
            }

            var results = new Dictionary<string, double>(
                StringComparer.OrdinalIgnoreCase);

            const int warmupIterations = 50;
            const int measurementIterations = 1000;

            foreach (INativeStringEngine engine in _engines)
            {
                // Warm-up: Essential for JIT and native library initialization.
                for (int i = 0; i < warmupIterations; i++)
                {
                    engine.Convert(input, choice);
                }

                // Measurement: Use a high-resolution stopwatch.
                Stopwatch stopwatch = Stopwatch.StartNew();

                for (int i = 0; i < measurementIterations; i++)
                {
                    engine.Convert(input, choice);
                }

                stopwatch.Stop();

                double averageMilliseconds =
                    stopwatch.Elapsed.TotalMilliseconds /
                    measurementIterations;

                results[engine.Name] = averageMilliseconds;
            }

            return Ok(results);
        }
    }
}