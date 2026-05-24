# -----------------------------------------------------------------------------
# Microsoft Azure Container Registry / .NET Microservices Template
# Project:      Hardware-Aware Case Conversion Engine
# Component:    Polyglot Build Orchestration (C++17 + .NET 8.0)
# Documentation: https://github.com/nitishhsinghhh/case-conversion-api
# -----------------------------------------------------------------------------
# VERSION HISTORY
# Version | Date       | Author       | Description
# --------|------------|--------------|----------------------------------------
# 1.0.0   | 2026-04-21 | Nitish Singh | Initial Release. Polyglot O11y integrated.
# 1.0.1   | 2026-04-22 | Nitish Singh | Fixed CMake source paths for Docker context.
# 1.0.2   | 2026-04-22 | Nitish Singh | Unified Monorepo Context Paths for Docker Build.
# -----------------------------------------------------------------------------
# ARCHITECTURAL STRATEGY:
# * Multi-Stage Isolation: Segregates build-time dependencies (CMake) from runtime.
# * ABI Integrity: Compiles native shared objects (.so) within target SDK.
# * Artifact Promotion: Manual injection of native binaries into .NET publish root.
# -----------------------------------------------------------------------------

FROM mcr.microsoft.com/dotnet/sdk:8.0 AS build

# Install native toolchain constraints
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Copy the entire repo context (Crucial for multi-stage relative folder parsing)
COPY . .

# 1. Purge potential local build dirtiness
RUN rm -rf Backend/CaseConversionAPI/CppLib/build

# 2. Build Native C++ Engine via explicit monorepo anchor paths
RUN cmake -S Backend/CaseConversionAPI/CppLib \
          -B Backend/CaseConversionAPI/CppLib/build \
          -DCMAKE_BUILD_TYPE=Release

RUN cmake --build Backend/CaseConversionAPI/CppLib/build --parallel $(nproc)

# 3. Publish Managed .NET API (Cd directly into context to prevent build path breakage)
RUN cd Backend/CaseConversionAPI/DotNetAPI && \
    dotnet publish -c Release -o /app/publish

# 4. ABI Bridge Promotion (Copying runtime native engine into publish directory root)
RUN cp Backend/CaseConversionAPI/CppLib/build/libProcessStringDLL.so /app/publish/


# --- STAGE 2: RUNTIME ENGINE ---
FROM mcr.microsoft.com/dotnet/aspnet:8.0 AS runtime
WORKDIR /app

# Bring forward only the polished production binaries
COPY --from=build /app/publish .

# Configure system environment to evaluate current directory first for dynamic linkage (.so loading)
ENV LD_LIBRARY_PATH=.

EXPOSE 8080
ENTRYPOINT ["dotnet", "DotNetAPI.dll"]