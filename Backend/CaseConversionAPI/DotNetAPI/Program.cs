/**************************************************************************************************
 * File         : Program.cs
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
 * Description  : Application bootstrap and initialization entry point for the Word Case REST API.
 *                Assembles dependency injection graphs, telemetry pipelines, security engines,
 *                and the HTTP middleware processing pipeline.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-11     Nitish Singh     Initial implementation of application bootstrap.
 * 1.1         2026-04-16     Nitish Singh     Optimized DI lifetime scopes and refined CORS handling.
 * 1.2         2026-04-18     Nitish Singh     Integrated OpenTelemetry (OTLP) for distributed tracing
 *                                             across unmanaged ABI boundaries; corrected namespaces.
 * 1.3         2026-05-06     Nitish Singh     Restructured middleware sequence for CORS preflight
 *                                             compliance and pinned server endpoints.
 * 1.4         2026-06-01     Nitish Singh     Containerized ecosystem updates: Decoupled hardcoded local
 *                                             bindings and parameterized OpenTelemetry target endpoints.
 * 1.5         2026-06-15     Nitish Singh     Standardized XML engineering metrics and enforced 
 *                                             production-grade JWT authentication/authorization schemas.
 **************************************************************************************************/

using System;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Mvc.Authorization;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.IdentityModel.Tokens;
using Microsoft.OpenApi.Models;
using OpenTelemetry;
using OpenTelemetry.Exporter;
using OpenTelemetry.Resources;
using OpenTelemetry.Trace;
using StringConversionAPI.Services;
using StringConversionAPI.Services.Native;
using StringConversionAPI.Services.Rust; 

WebApplicationBuilder builder = WebApplication.CreateBuilder(args);

// ================================================================================================
// 1. SERVICES CONFIGURATION (Dependency Injection Engine)
// ================================================================================================

string otelEndpoint = builder.Configuration["OTEL_EXPORTER_OTLP_ENDPOINT"] ?? "http://localhost:4317";
const string ServiceName = "CaseConversion-Gateway";

builder.Services.AddOpenTelemetry()
    .ConfigureResource(resource => resource.AddService(ServiceName))
    .WithTracing(tracing => tracing
        .AddSource("CaseConversion.Engine")
        .AddAspNetCoreInstrumentation()
        .AddHttpClientInstrumentation()
        .AddOtlpExporter(options =>
        {
            options.Endpoint = new Uri(otelEndpoint);
            options.ExportProcessorType = ExportProcessorType.Batch;
        }));

IConfigurationSection jwtSettings = builder.Configuration.GetSection("Jwt");
byte[] key = Encoding.UTF8.GetBytes("SuperSecretKey_NitishSingh_2026_HighPerformance");

builder.Services.AddAuthentication(options =>
{
    options.DefaultAuthenticateScheme = JwtBearerDefaults.AuthenticationScheme;
    options.DefaultChallengeScheme = JwtBearerDefaults.AuthenticationScheme;
})
.AddJwtBearer(options =>
{
    options.Events = new JwtBearerEvents
    {
        OnTokenValidated = context =>
        {
            // Log successful authentications via underlying tracing blocks if necessary
            return Task.CompletedTask;
        },
        OnAuthenticationFailed = context =>
        {
            return Task.CompletedTask;
        }
    };

    options.TokenValidationParameters = new TokenValidationParameters
    {
        ValidateIssuer = true,
        ValidateAudience = true,
        ValidateLifetime = true,
        ValidateIssuerSigningKey = true,
        ValidIssuer = jwtSettings["Issuer"] ?? "CaseConversion-Gateway",
        ValidAudience = jwtSettings["Audience"] ?? "CaseConversion-Client",
        IssuerSigningKey = new SymmetricSecurityKey(key),
        ClockSkew = TimeSpan.Zero
    };
});

builder.Services.AddControllers(options =>
{
    // Bypass authorization blocks only during localized structural integration testing runs
    if (builder.Environment.IsEnvironment("Testing"))
    {
        options.Filters.Add(new AllowAnonymousFilter());
    }
})
.AddJsonOptions(options =>
{
    options.JsonSerializerOptions.PropertyNameCaseInsensitive = true;
    options.JsonSerializerOptions.PropertyNamingPolicy = System.Text.Json.JsonNamingPolicy.CamelCase;
});

builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen(c =>
{
    c.SwaggerDoc("v3", new OpenApiInfo 
    { 
        Title = "Word Case Polyglot API", 
        Version = "v3",
        Description = "High-performance processing gateway leveraging C++ interop layers and distributed telemetry."
    });

    c.AddSecurityDefinition("Bearer", new OpenApiSecurityScheme
    {
        Description = "JWT Authorization header using the Bearer scheme. Form: \"Authorization: Bearer {token}\"",
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


builder.Services.AddSingleton<ITokenService, TokenService>();
builder.Services.AddSingleton<CppEngineService>();
builder.Services.AddSingleton<RustEngineService>();
builder.Services.AddSingleton<INativeStringEngine, CppEngineService>();
builder.Services.AddSingleton<INativeStringEngine, RustEngineService>();

builder.Services.AddScoped<INativeStringEngine>(serviceProvider => 
{
    var config = serviceProvider.GetRequiredService<IConfiguration>();
    var provider = config["NativeEngineSettings:Provider"] ?? "cpp";
    
    return provider.ToLower() == "rust" 
        ? serviceProvider.GetRequiredService<RustEngineService>() 
        : serviceProvider.GetRequiredService<CppEngineService>();
});

WebApplication app = builder.Build();

// ================================================================================================
// 2. MIDDLEWARE PIPELINE (Deterministic Processing Order)
// ================================================================================================

// Enforce standard OpenAPI definition exposing strategies
app.UseSwagger();
app.UseSwaggerUI(c =>
{
    c.SwaggerEndpoint("/swagger/v3/swagger.json", "Word Case API v3 Documentation");
    c.RoutePrefix = string.Empty;
});

// Routing must precede CORS and Security filters
app.UseRouting();
app.UseCors();

if (!app.Environment.IsEnvironment("Testing"))
{
    app.UseAuthentication();
    app.UseAuthorization();
}

app.MapControllers();

app.Run();

/// <summary>
/// Exposes assembly hooks targeting runtime components inside integration testing fixtures.
/// </summary>
public partial class Program { }