/**************************************************************************************************
 * File         : LoginRequest.cs
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
 * Description  : Data transfer object (DTO) using a positional record type to encapsulate
 *                incoming authentication assertions.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-05-23     Nitish Singh     Refactored into an immutable positional record.
 **************************************************************************************************/

using System.ComponentModel.DataAnnotations;

namespace StringConversionAPI.Authentication.Models
{
    /// <summary>
    /// Represents the incoming authentication payload
    /// submitted by a client requesting JWT issuance.
    /// </summary>
    public sealed record LoginRequest
    (
        [Required]
        [MinLength(3)]
        string Username,

        [Required]
        [MinLength(4)]
        string Password
    );
}