/**************************************************************************************************
 * File         : TokenService.cs
 *
 * Copyright    : (c) 2016–2026 Nitish Singh. All rights reserved.
 * This material may be reproduced for teaching and learning purposes only.
 * It is not to be used in industry or for commercial purposes.
 *
 * Class        : TokenService
 * Interface    : ITokenService
 *
 * Description  : Provides JWT token generation services for the Case Conversion API.
 * Handles secure claim construction, role-based authorization claims,
 * HMAC-SHA256 signing, issuer/audience configuration, and token expiration.
 *
 * Notes        : - Implements stateless JWT generation for authentication workflows.
 *               - Uses symmetric key cryptography (HMAC-SHA256).
 *               - Supports role-based authorization claims.
 *               - Designed for ASP.NET Core authentication middleware integration.
 *               - Compatible with distributed API gateway deployments.
 *               - Security key should be externally configured via IConfiguration.
 *               - Tokens include JTI claim for replay tracking and observability.
 *               - Intended for short-lived access token generation.
 *
 * Security     : - Enforces signed JWT tokens using HMAC-SHA256.
 *               - Prevents unsigned token generation.
 *               - Supports issuer/audience validation workflows.
 *               - Intended to operate behind HTTPS transport security.
 *               - Compatible with zero-trust API authentication architecture.
 *
 * Thread Safe  : Yes (stateless service)
 * Complexity   : O(n) relative to number of roles
 * API Status   : Stable
 * Exception Safety : Strong Guarantee
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-05-18     Nitish Singh     Initial JWT token service implementation
 **************************************************************************************************/

using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;
using System.Text;
using Microsoft.IdentityModel.Tokens;

namespace StringConversionAPI.Services
{
    public interface ITokenService
    {
        string GenerateToken(string username, IEnumerable<string> roles);
    }

    public class TokenService : ITokenService
    {
        private readonly IConfiguration _config;
        public TokenService(IConfiguration config) => _config = config;

        public string GenerateToken(string username, IEnumerable<string> roles)
        {
            var jwtSettings = _config.GetSection("Jwt");
            // Use the exact key from the Program.cs
            var key = new SymmetricSecurityKey(Encoding.UTF8.GetBytes("SuperSecretKey_NitishSingh_2026_HighPerformance"));
            var creds = new SigningCredentials(key, SecurityAlgorithms.HmacSha256);

            var claims = new List<Claim>
            {
                new Claim(JwtRegisteredClaimNames.Sub, username),
                new Claim(JwtRegisteredClaimNames.Jti, Guid.NewGuid().ToString()),
                new Claim("name", username)
            };

            foreach (var role in roles)
            {
                claims.Add(new Claim(ClaimTypes.Role, role));
            }

            var token = new JwtSecurityToken(
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