/**************************************************************************************************
 * File         : ITokenService.cs
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
 * Description  : Defines the contract for processing authentication assertions and provisioning 
 *                short-lived access tokens.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-05-23     Nitish Singh     Initial interface definition.
 **************************************************************************************************/

using System.Collections.Generic;

namespace StringConversionAPI.Services
{
    /// <summary>
    /// Defines the contract for processing authentication assertions and provisioning short-lived access tokens.
    /// </summary>
    public interface ITokenService
    {
        /// <summary>
        /// Generates a cryptographically signed JSON Web Token (JWT) with authorization claims.
        /// </summary>
        /// <param name="username">The unique identifier of the authenticated user or application client.</param>
        /// <param name="roles">The collection of logical roles associated with the authenticated user.</param>
        /// <returns>A string representing the serialized compact JWT bearer token.</returns>
        string GenerateToken(string username, IEnumerable<string> roles);
    }
}