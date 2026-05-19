/**************************************************************************************************
 * File         : ConvertResponse.cs
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
 * Description  : Data Transfer Object (DTO) defining the outbound network schema for
 *                string conversion operations. Encapsulates transformed string metadata.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-11     Nitish Singh     Initial implementation of response data contract.
 * 1.1         2026-04-20     Nitish Singh     Added ConvertedText property to resolve compilation 
 *                                             error CS0117. Mapped Output as an alias property 
 *                                             to preserve backward compatibility.
 **************************************************************************************************/

using System.Text.Json.Serialization;

namespace StringConversionAPI.Models
{
    /// <summary>
    /// Represents the outbound network contract returned to clients following a case conversion operation.
    /// This object structures the output payload generated across the native execution boundary.
    /// </summary>
    public sealed class ConvertResponse
    {
        /// <summary>
        /// Gets or sets the original raw string submitted by the client.
        /// </summary>
        /// <value>
        /// A <see cref="string"/> containing the pristine input. Defaults to <see cref="string.Empty"/>.
        /// </value>
        [JsonPropertyName("input")]
        public string Input { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the algorithm strategy identifier chosen for execution.
        /// </summary>
        /// <value>
        /// An <see cref="int"/> mapping to the core processing engine operational routine.
        /// </value>
        [JsonPropertyName("choiceId")]
        public int Choice { get; set; }

        /// <summary>
        /// Gets or sets the final processed text received from the underlying unmanaged engine.
        /// </summary>
        /// <value>
        /// A <see cref="string"/> representing the transformed text payload. Defaults to <see cref="string.Empty"/>.
        /// </value>
        [JsonPropertyName("convertedText")]
        public string ConvertedText { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets a calculated alias property wrapping <see cref="ConvertedText"/> 
        /// to support legacy interface definitions.
        /// </summary>
        /// <value>
        /// A proxy <see cref="string"/> redirecting directly to the primary target string.
        /// </value>
        /// <remarks>
        /// Decorated with <see cref="JsonIgnoreAttribute"/> to exclude this duplication 
        /// from serialization payloads, avoiding structural collisions with external clients.
        /// </remarks>
        [JsonIgnore]
        public string Output
        {
            get => ConvertedText;
            set => ConvertedText = value;
        }
    }
}