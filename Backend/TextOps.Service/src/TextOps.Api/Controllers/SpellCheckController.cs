/**************************************************************************************************
 * File         : SpellCheckController.cs
 *
 * Copyright    : (c) 2016–2026 Nitish Singh. All rights reserved.
 * License      : Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Description  : REST API Gateway endpoint exposing the high-performance native unmanaged C++
 * spellchecking engine wrappers. Handles trace tracking contexts, explicit security payload 
 * boundaries, and standard object mapping structures.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author         Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-05-25     Nitish Singh   Initial REST API Controller layout implementation.
 **************************************************************************************************/

using System;
using System.ComponentModel.DataAnnotations;
using System.Diagnostics;
using System.Net.Mime;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using TextOps.Api.Services.Interop;

namespace TextOps.Api.Controllers
{
    /// <summary>
    /// Contract definition for an explicit dictionary addition request payload block.
    /// </summary>
    public record DictionaryExtensionRequest(
        [Required(AllowEmptyStrings = false)]
        [StringLength(1024, MinimumLength = 1, ErrorMessage = "Word length token bound violation limits.")]
        string Word
    );

    [ApiController]
    [Route("api/v1/[controller]")]
    [Produces(MediaTypeNames.Application.Json)]
    public class SpellCheckController : ControllerBase
    {
        private readonly IManagedSpellCheckService _spellCheckService;
        private const int MaxWordBufferBoundary = 1024;

        public SpellCheckController(IManagedSpellCheckService spellCheckService)
        {
            _spellCheckService = spellCheckService ?? throw new ArgumentNullException(nameof(spellCheckService));
        }

        /// <summary>
        /// Validates a single word parameter token using the underlying multi-tier unmanaged C++ search index structure.
        /// </summary>
        /// <param name="word">The targeted raw string phrase token requiring algorithmic verification pass verification.</param>
        /// <returns>An immutable structured response containing accuracy status state matching criteria metrics.</returns>
        [HttpGet("check")]
        [ProducesResponseType(typeof(SpellCheckResult), StatusCodes.Status200OK)]
        [ProducesResponseType(typeof(ValidationProblemDetails), StatusCodes.Status400BadRequest)]
        [ProducesResponseType(typeof(ProblemDetails), StatusCodes.Status500InternalServerError)]
        public IActionResult CheckWord([FromQuery] string word)
        {
            if (string.IsNullOrWhiteSpace(word))
            {
                ModelState.AddModelError(nameof(word), "The validation word parameter target segment token cannot be empty.");
                return ValidationProblem();
            }

            if (word.Length > MaxWordBufferBoundary)
            {
                ModelState.AddModelError(nameof(word), $"The word parameter length exceeds the rigid security validation limits of {MaxWordBufferBoundary} characters.");
                return ValidationProblem();
            }

            try
            {
                var result = _spellCheckService.VerifyWord(word);
                return Ok(result);
            }
            // Most specific first
            catch (ObjectDisposedException ex)
            {
                return CreateProblemDetails(ex, StatusCodes.Status500InternalServerError);
            }
            catch (InvalidOperationException ex)
            {
                return CreateProblemDetails(ex, StatusCodes.Status500InternalServerError);
            }
            // Most general (base interop exception) last
            catch (System.Runtime.InteropServices.ExternalException ex)
            {
                return CreateProblemDetails(ex, StatusCodes.Status500InternalServerError);
            }
        }

        private IActionResult CreateProblemDetails(Exception ex, int statusCode)
        {
            return StatusCode(statusCode, new ProblemDetails
            {
                Status = statusCode,
                Title = "Native Execution Boundary Fault Encountered",
                Detail = ex.Message,
                Instance = HttpContext.Request.Path,
                Extensions = { ["traceId"] = Activity.Current?.Id ?? HttpContext.TraceIdentifier }
            });
        }

        /// <summary>
        /// Commits an explicit custom validation phrase token into the native persistent store data structure files.
        /// </summary>
        /// <param name="request">The strongly-typed envelope configuration layout block tracking information context values.</param>
        [HttpPost("dictionary/extend")]
        [Consumes(MediaTypeNames.Application.Json)]
        [ProducesResponseType(StatusCodes.Status202Accepted)]
        [ProducesResponseType(typeof(ValidationProblemDetails), StatusCodes.Status400BadRequest)]
        public IActionResult AddWord([FromBody] DictionaryExtensionRequest request)
        {
            // Safeguard handling against malformed routing requests
            if (request == null || string.IsNullOrWhiteSpace(request.Word))
            {
                ModelState.AddModelError(nameof(request.Word), "Payload object target parameter structure definitions missing target criteria items.");
                return ValidationProblem();
            }

            _spellCheckService.AddWordToPersonalDictionary(request.Word);
            return Accepted();
        }
    }
}