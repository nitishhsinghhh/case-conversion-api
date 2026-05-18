/**************************************************************************************************
 * File         : Program.cs
 *
 * Copyright    : (c) 2016–2026 nitishhsinghh. All rights reserved.
 * This material may be reproduced for teaching and learning purposes only.
 * It is not to be used in industry or for commercial purposes.
 *
 * Description  : Application entry point for the Word Case REST API.
 * Configures ASP.NET Core middleware pipeline, dependency injection,
 * CORS policies, OpenTelemetry observability, and Swagger.
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-11     Nitish Singh     Initial implementation of application bootstrap
 * 1.1         2026-04-16     Nitish Singh     Optimized DI lifetime and refined CORS handling.
 * 1.2         2026-04-18     Nitish Singh     Integrated OpenTelemetry (OTLP) for distributed 
 *                                             tracing across the native ABI boundary. Corrected 
 *                                             OTLP namespace.
 * 1.3         2026-05-06     Nitish Singh     Restructured middleware pipeline for CORS preflight
 *                                             support and pinned Kestrel to Port 5050.
 * 1.4         2026-06-01     Nitish Singh     Refactored for Containerization: Removed hardcoded 
 *                                             localhost bindings and parameterized OTEL endpoints.
 * 1.5         2026-06-15     Nitish Singh     Standardized XML documentation and enforced
 *                                             JWT Authentication & Authorization schemas.
 **************************************************************************************************/

using Microsoft.OpenApi.Models;
using StringConversionAPI.Services;
using OpenTelemetry.Resources;
using OpenTelemetry.Trace;
using OpenTelemetry; 
using OpenTelemetry.Exporter;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.IdentityModel.Tokens;
using System.Text; 

var builder = WebApplication.CreateBuilder(args);

// ================================================================================================
// 1. SERVICES CONFIGURATION (Dependency Injection)
// ================================================================================================

/// <summary>
/// Configure OpenTelemetry for distributed tracing. 
/// Essential for monitoring cross-boundary calls between .NET and Native C++.
/// </summary>
var otelEndpoint = builder.Configuration["OTEL_EXPORTER_OTLP_ENDPOINT"] ?? "http://localhost:4317";
const string serviceName = "CaseConversion-Gateway";

builder.Services.AddOpenTelemetry()
    .ConfigureResource(resource => resource.AddService(serviceName))
    .WithTracing(tracing => tracing
        .AddSource("CaseConversion.Engine") 
        .AddAspNetCoreInstrumentation()      
        .AddHttpClientInstrumentation()      
        .AddOtlpExporter(options =>
        {
            options.Endpoint = new Uri(otelEndpoint);
            options.ExportProcessorType = ExportProcessorType.Batch;    // Changed to Batch for performance
        }));

/// <summary>
/// Configure JWT Authentication.
/// Uses a high-performance symmetric key for local token validation.
/// </summary>
var jwtSettings = builder.Configuration.GetSection("Jwt");
var key = Encoding.UTF8.GetBytes("SuperSecretKey_NitishSingh_2026_HighPerformance");

builder.Services.AddAuthentication(options =>
{
    options.DefaultAuthenticateScheme = JwtBearerDefaults.AuthenticationScheme;
    options.DefaultChallengeScheme = JwtBearerDefaults.AuthenticationScheme;
})
.AddJwtBearer(options =>
{   options.Events = new JwtBearerEvents
    {
        OnTokenValidated = context => 
        {
            Console.WriteLine("Token validated successfully!");
            return Task.CompletedTask;
        },
        OnAuthenticationFailed = context =>
        {
            Console.WriteLine($"Auth Failed: {context.Exception.Message}");
            return Task.CompletedTask;
        }
    };
    
    options.TokenValidationParameters = new TokenValidationParameters
    {
        ValidateIssuer = true,
        ValidateAudience = true,
        ValidateLifetime = true,
        ValidateIssuerSigningKey = true,
        ValidIssuer = jwtSettings["Issuer"],
        ValidAudience = jwtSettings["Audience"],
        IssuerSigningKey = new SymmetricSecurityKey(key),
        ClockSkew = TimeSpan.Zero 
    };
});

/// <summary>
/// Add Controller services with optimized JSON serialization settings.
/// </summary>
builder.Services.AddControllers(options =>
{
    // Bypass authentication ONLY during Integration Testing
    if (builder.Environment.IsEnvironment("Testing"))
    {
        options.Filters.Add(new Microsoft.AspNetCore.Mvc.Authorization.AllowAnonymousFilter());
    }
})
.AddJsonOptions(options =>
{
    options.JsonSerializerOptions.PropertyNameCaseInsensitive = true;
    options.JsonSerializerOptions.PropertyNamingPolicy = System.Text.Json.JsonNamingPolicy.CamelCase;
});

/// <summary>
/// Configure Swagger/OpenAPI with Security Definitions for Bearer Auth.
/// </summary>
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen(c =>
{
    c.SwaggerDoc("v3", new OpenApiInfo { Title = "Word Case API", Version = "v3" });

    // Optional: Adds the Authorize button inside Swagger UI for easy JWT testing
    c.AddSecurityDefinition("Bearer", new OpenApiSecurityScheme
    {
        Description = "JWT Authorization header using the Bearer scheme. Example: \"Authorization: Bearer {token}\"",
        Name = "Authorization",
        In = ParameterLocation.Header,
        Type = SecuritySchemeType.ApiKey,
        Scheme = "Bearer"
    });
    c.AddSecurityRequirement(new OpenApiSecurityRequirement
    {
        {
            new OpenApiSecurityScheme
            {
                Reference = new OpenApiReference { Type = ReferenceType.SecurityScheme, Id = "Bearer" }
            },
            Array.Empty<string>()
        }
    });
});

/// <summary>
/// Configure Global CORS policy to allow cross-origin requests from front-end consumers.
/// </summary>
builder.Services.AddCors(options =>
{
    options.AddDefaultPolicy(policy =>
    {
        policy.AllowAnyOrigin()
              .AllowAnyHeader()
              .AllowAnyMethod()
              .SetPreflightMaxAge(TimeSpan.FromMinutes(30)); 
    });
});

/// <summary>
/// Configure Core Business Services.
/// </summary>
builder.Services.AddSingleton<ITokenService, TokenService>(); // Register the new Token Service
builder.Services.AddSingleton<ProcessStringService>();

var app = builder.Build();

// ================================================================================================
// 2. MIDDLEWARE PIPELINE (Request Processing)
// ================================================================================================

app.UseRouting();
app.UseCors(); 

// Documentation middleware
app.UseSwagger();
app.UseSwaggerUI(c =>
{
    c.SwaggerEndpoint("/swagger/v3/swagger.json", "Word Case API v3 Documentation");
    c.RoutePrefix = string.Empty; 
});

// Security middleware

if (!app.Environment.IsEnvironment("Testing"))
{
    app.UseAuthentication();
    app.UseAuthorization();
}
else
{
    Console.WriteLine("Running in Testing environment: Skipping Authentication & Authorization middleware.");
}

// Endpoint mapping
app.MapControllers();

/// <summary>
/// Start the Kestrel web server.
/// </summary>
app.Run();

/// <summary>
/// Exposed for integration testing purposes.
/// </summary>
public partial class Program { }