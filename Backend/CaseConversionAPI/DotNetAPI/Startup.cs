/**************************************************************************************************
 * File         : Startup.cs
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
 * Description  : Legacy component configuration class managing services orchestration and HTTP 
 *                request pipeline handling. Retained for backwards-compatibility or selective 
 *                bootstrapping schemas alongside modern unified hosting models.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-11     Nitish Singh     Initial implementation of startup configuration.
 **************************************************************************************************/

using System;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using StringConversionAPI.Services;

namespace StringConversionAPI
{
    /// <summary>
    /// Configures essential application services and constructs the HTTP execution middleware chain.
    /// </summary>
    public sealed class Startup
    {
        /// <summary>
        /// Gets the application key-value configuration tree root.
        /// </summary>
        public IConfiguration Configuration { get; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Startup"/> class.
        /// </summary>
        /// <param name="configuration">The centralized configuration provider engine instance.</param>
        public Startup(IConfiguration configuration)
        {
            Configuration = configuration ?? throw new ArgumentNullException(nameof(configuration));
        }

        /// <summary>
        /// Configures runtime dependencies into the host service collection container.
        /// </summary>
        /// <param name="services">The target service collection to register dependencies within.</param>
        public void ConfigureServices(IServiceCollection services)
        {
            if (services == null)
            {
                throw new ArgumentNullException(nameof(services));
            }

            // Register core high-performance unmanaged boundary service handler
            services.AddSingleton<ProcessStringService>();
            
            services.AddControllers();
            services.AddEndpointsApiExplorer();
            services.AddSwaggerGen();
        }

        /// <summary>
        /// Configures the runtime HTTP request pipeline based on environment characteristics.
        /// </summary>
        /// <param name="app">The builder utility managing application pipeline definitions.</param>
        /// <param name="env">The environment descriptor capturing deployment traits.</param>
        public void Configure(WebApplication app, IWebHostEnvironment env)
        {
            if (app == null)
            {
                throw new ArgumentNullException(nameof(app));
            }

            if (env == null)
            {
                throw new ArgumentNullException(nameof(env));
            }

            if (env.IsDevelopment())
            {
                app.UseSwagger();
                app.UseSwaggerUI();
            }

            app.UseRouting();
            app.UseAuthorization();
            app.MapControllers();
        }
    }
}