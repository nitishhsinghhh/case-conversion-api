using Microsoft.AspNetCore.Builder;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.OpenApi.Models;
using TextOps.Api.Services.Interop;

var builder = WebApplication.CreateBuilder(args);

// 1. ADD SERVICES TO THE CONTAINER

// Add required abstractions for API exploration and Controller endpoint routing
builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();

// Configure the OpenAPI/Swagger generation engine
builder.Services.AddSwaggerGen(options =>
{
    options.SwaggerDoc("v1", new OpenApiInfo
    {
        Title = "Text Operations Spellcheck API",
        Version = "v1",
        Description = "High-performance processing gateway leveraging native unmanaged C++ spellchecking engines."
    });
});

// Register the native interop orchestration manager as a structured singleton service target
builder.Services.AddSingleton<IManagedSpellCheckService>(sp => 
    new ManagedSpellCheckService("/usr/share/hunspell/en_US"));

var app = builder.Build();

// 2. CONFIGURE THE HTTP REQUEST PIPELINE

// Always expose Swagger documentation files and the interactive UI playground
app.UseSwagger();
app.UseSwaggerUI(options =>
{
    options.SwaggerEndpoint("/swagger/v1/swagger.json", "TextOps Spellcheck API v1");
    options.RoutePrefix = string.Empty; // Mounts the Swagger dashboard documentation interface directly at the root URL
});

app.UseAuthorization();

// Route incoming requests directly to your SpellCheckController endpoints
app.MapControllers();

app.Run();