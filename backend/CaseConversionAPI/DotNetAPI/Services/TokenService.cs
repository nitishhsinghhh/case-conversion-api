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
            // Use the exact key from your Program.cs
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