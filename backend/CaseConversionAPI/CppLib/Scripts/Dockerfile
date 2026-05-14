# -----------------------------------------------------------------------------
# Microsoft Azure Container Registry / .NET Microservices Template
# Project:     Hardware-Aware Case Conversion Engine
# Component:   Multi-Platform Native Orchestrator (Linux + Windows)
# Documentation: https://github.com/nitishhsinghhh/case-conversion-api
# -----------------------------------------------------------------------------
# VERSION HISTORY
# Version | Date       | Author       | Description
# --------|------------|--------------|----------------------------------------
# 1.0.0   | 2026-05-14 | Nitish Singh | Initial Orchestration Layer.
# 1.1.0   | 2026-05-14 | Nitish Singh | Integrated MinGW-w64 for Windows cross-builds.
# -----------------------------------------------------------------------------
# ARCHITECTURAL STRATEGY:
# * Headless Build Vessel: Designed to output binaries, not to run as a service.
# * Cross-Compilation: Leverages MinGW-w64 to target Windows from a Linux host.
# * Artifact Extraction: Exposes the build directory for host-side consumption.
# -----------------------------------------------------------------------------

# --- STAGE 1: Build Environment (Native Toolchains Only) ---
FROM mcr.microsoft.com/dotnet/sdk:8.0 AS build-env

# Install C++ Native Toolchains (GCC for Linux, MinGW for Windows)
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    mingw-w64 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Copy the entire repo context to preserve script and source paths
COPY . .

# Ensure orchestrator script is executable
RUN chmod +x backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh

# --- STEP 1: Build Linux Artifacts (.so) ---
RUN ./backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh ubuntu-latest

# --- STEP 2: Build Windows Artifacts (.dll via MinGW) ---
RUN ./backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh windows-latest

# -----------------------------------------------------------------------------
# STAGE 2: Export Layer (Minimal Delivery Vessel)
# -----------------------------------------------------------------------------
FROM mcr.microsoft.com/dotnet/aspnet:8.0 AS runtime
WORKDIR /app

# Export the raw C++ build artifacts for the host to extract via 'docker cp'
# This bypasses managed code publish to focus on native binary delivery.
COPY --from=build-env /src/backend/CaseConversionAPI/CppLib/build /src/backend/CaseConversionAPI/CppLib/build

# Note: No ENTRYPOINT defined as this container is utilized as an artifact source.