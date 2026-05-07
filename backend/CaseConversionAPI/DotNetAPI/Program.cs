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
 * tracing across the native ABI boundary. Corrected OTLP namespace.
 * 1.3         2026-05-06     Nitish Singh     Restructured middleware pipeline for CORS preflight
 * support and pinned Kestrel to Port 5050.
 **************************************************************************************************/

using Microsoft.OpenApi.Models;
using StringConversionAPI.Services;
using OpenTelemetry.Resources;
using OpenTelemetry.Trace;
using OpenTelemetry; 
using OpenTelemetry.Exporter; 

var builder = WebApplication.CreateBuilder(args);

// --- 1. OpenTelemetry & Observability ---
const string serviceName = "CaseConversion-Gateway";

builder.Services.AddOpenTelemetry()
    .ConfigureResource(resource => resource.AddService(serviceName))
    .WithTracing(tracing => tracing
        .AddSource("CaseConversion.Engine") 
        .AddAspNetCoreInstrumentation()      
        .AddHttpClientInstrumentation()      
        .AddOtlpExporter(options =>
        {
            options.Endpoint = new Uri("http://localhost:4317");
            options.ExportProcessorType = ExportProcessorType.Simple;
        }));

// --- 2. Controller & Documentation Services ---
builder.Services.AddControllers()
    .AddJsonOptions(options =>
    {
        options.JsonSerializerOptions.PropertyNameCaseInsensitive = true;
        options.JsonSerializerOptions.PropertyNamingPolicy = System.Text.Json.JsonNamingPolicy.CamelCase;
    });

builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen(c =>
{
    c.SwaggerDoc("v1", new OpenApiInfo { Title = "Word Case API", Version = "v1" });
});

// --- 3. Optimized CORS Policy ---
builder.Services.AddCors(options =>
{
    options.AddDefaultPolicy(policy =>
    {
        policy.AllowAnyOrigin()
              .AllowAnyHeader()
              .AllowAnyMethod()
              .SetPreflightMaxAge(TimeSpan.FromMinutes(10)); 
    });
});

// --- 4. Core Business Services ---
builder.Services.AddSingleton<ProcessStringService>();

// Pinning the WebHost to Port 5050 to avoid macOS AirPlay (Port 5000) conflict
builder.WebHost.UseUrls("http://localhost:5050"); 

var app = builder.Build();

// --- 5. Middleware Pipeline (Strict Order Required) ---

// A. Enable Routing first so CORS can identify the endpoint
app.UseRouting();

// B. Enable CORS immediately after Routing to handle Preflight (OPTIONS) requests
app.UseCors(); 

// C. Documentation Services
app.UseSwagger();
app.UseSwaggerUI(c =>
{
    c.SwaggerEndpoint("/swagger/v1/swagger.json", "Word Case API v1");
    c.RoutePrefix = string.Empty; 
});

// D. Security & Mapping
app.UseAuthorization();
app.MapControllers();

// E. Execution Boundary
if (app.Environment.EnvironmentName != "Testing")
{
    app.Run("http://localhost:5050");
}

public partial class Program { }