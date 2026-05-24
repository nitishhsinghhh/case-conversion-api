/**************************************************************************************************
 * File         : ConvertRequest.cs
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
 * Description  : Data Transfer Object (DTO) defining the schema for incoming text 
 *                transformation requests targeting the Case Conversion Gateway.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-11     Nitish Singh     Initial implementation of request data contract.
 * 1.1         2026-04-20     Nitish Singh     Converted Text property to a nullable type reference 
 *                                             to prevent automatic framework-level 400 Bad Request
 *                                             errors, ensuring empty or null states cleanly propagate
 *                                             to downstream application service validation layers.
 **************************************************************************************************/

using System.ComponentModel.DataAnnotations;

namespace StringConversionAPI.Models
{
    /// <summary>
    /// Represents the network request contract for case conversion operations.
    /// This object encapsulates client payloads transmitted over the ASP.NET Core web boundary.
    /// </summary>
    public sealed class ConvertRequest
    {
        /// <summary>
        /// Gets or sets the target text payload requiring conversion or manipulation.
        /// </summary>
        /// <value>
        /// A <see cref="string"/> containing the source text. Defaults to <see cref="string.Empty"/>.
        /// </value>
        /// <remarks>
        /// Marked explicitly as a nullable reference type (<c>string?</c>) to bypass the strict 
        /// non-nullable model binding assertions enforced implicitly by the .NET 8 controller 
        /// runtimes. This configuration shifts data validation responsibility directly onto downstream 
        /// processing services.
        /// </remarks>
        public string? Text { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the unique conversion strategy identifier applied to the input payload.
        /// </summary>
        /// <value>
        /// An <see cref="int"/> representing the selected algorithmic transformation mode 
        /// mapping to underlying unmanaged processing engine routines.
        /// </value>
        /// <remarks>
        /// This index corresponds directly to specific conversion operations optimized inside 
        /// the cross-boundary execution layers.
        /// </remarks>
        [Required]
        public int Choice { get; set; }
    }
}