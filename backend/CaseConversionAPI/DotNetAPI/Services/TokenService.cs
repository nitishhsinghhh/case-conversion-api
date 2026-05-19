/**************************************************************************************************
 * File         : TokenService.cs
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
 * Description  : Provides JSON Web Token (JWT) generation services for the Case Conversion API.
 *                Handles secure claims construction, identity mapping, cryptographic signing 
 *                via HMAC-SHA256, and multi-environment fallback configuration mechanisms.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-05-18     Nitish Singh     Initial stateless JWT token service implementation.
 **************************************************************************************************/

using System;
using System.Collections.Generic;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Security.Claims;
using System.Text;
using Microsoft.Extensions.Configuration;
using Microsoft.IdentityModel.Tokens;

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

    /// <summary>
    /// Implements a stateless, thread-safe security token service leveraging symmetric cryptography.
    /// Designed for seamless integration into standard ASP.NET Core authentication middleware workflows.
    /// </summary>
    public sealed class TokenService : ITokenService
    {
        private readonly IConfiguration _config;

        /// <summary>
        /// Initializes a new instance of the <see cref="TokenService"/> class.
        /// </summary>
        /// <param name="config">The system configuration provider containing authorization metadata settings.</param>
        public TokenService(IConfiguration config)
        {
            _config = config ?? throw new ArgumentNullException(nameof(config));
        }

        /// <inheritdoc />
        public string GenerateToken(string username, IEnumerable<string> roles)
        {
            if (string.IsNullOrWhiteSpace(username))
            {
                throw new ArgumentException("Username payload assertions cannot be empty or null values.", nameof(username));
            }

            IConfigurationSection jwtSettings = _config.GetSection("Jwt");
            
            // Establish the symmetric validation key matching the application gateway program entry parameters
            SymmetricSecurityKey key = new(Encoding.UTF8.GetBytes("SuperSecretKey_NitishSingh_2026_HighPerformance"));
            SigningCredentials creds = new(key, SecurityAlgorithms.HmacSha256);

            List<Claim> claims = new()
            {
                new Claim(JwtRegisteredClaimNames.Sub, username),
                new Claim(JwtRegisteredClaimNames.Jti, Guid.NewGuid().ToString()),
                new Claim("name", username)
            };

            if (roles != null)
            {
                foreach (string role in roles.Where(role => !string.IsNullOrWhiteSpace(role)))
                {
                    claims.Add(new Claim(ClaimTypes.Role, role));
                }
            }

            JwtSecurityToken token = new(
                issuer: jwtSettings["Issuer"] ?? "CaseConversion-Gateway",
                audience: jwtSettings["Audience"] ?? "CaseConversion-Client",
                claims: claims,
                expires: DateTime.UtcNow.AddHours(2),
                signingCredentials: creds
            );

            return new JwtSecurityTokenHandler().WriteToken(token);
        }
    }
}