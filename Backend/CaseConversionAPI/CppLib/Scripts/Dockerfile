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

ARG TARGET_PLATFORM=linux/amd64
FROM --platform=${TARGET_PLATFORM} mcr.microsoft.com/dotnet/sdk:8.0 AS build-env

# Install C++ Native Toolchains, Wine, and Xvfb for testing
RUN dpkg --add-architecture i386 && \
    apt-get update && apt-get install -y \
    build-essential \
    cmake \
    mingw-w64 \
    wine \
    wine32 \
    wine64 \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Copy the entire repo context
COPY . .

# Ensure orchestrator script is executable
RUN chmod +x Backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh

# --- STEP 1: Build Linux Artifacts (.so) ---
RUN ./Backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh ubuntu-latest

# --- STEP 2: Build Windows Artifacts (.dll via MinGW) ---
RUN ./Backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh windows-latest

# --- STEP 3: Verification Layer (Internal Tests) ---
WORKDIR /src/Backend/CaseConversionAPI/CppLib/build/windows-latest

ENV WINEPREFIX=/root/.wine
ENV WINEARCH=win64
ENV WINEDEBUG=-all
ENV WINEDLLOVERRIDES="mscoree,mshtml="

# Run Windows test suite via Wine
# Using absolute path to /usr/bin/xvfb-run ensures command execution
RUN echo "===== Running Windows GoogleTests via Wine =====" && \
    /usr/bin/xvfb-run --server-args="-screen 0 1024x768x24" \
    /usr/lib/wine/wine64 ./runTests.exe --gtest_color=yes 2>&1 | tee gtest_internal.log && \
    echo "===== Windows GoogleTests Passed =====" \
    || { \
        echo "=== WINDOWS GOOGLETEST CRASH LOG ==="; \
        cat gtest_internal.log; \
        exit 1; \
    }
    
# Reset context for artifact extraction
WORKDIR /src

# -----------------------------------------------------------------------------
# STAGE 2: Export Layer (Minimal Delivery Vessel)
# -----------------------------------------------------------------------------
# Re-import target architecture variable for the final runtime stage context
ARG TARGET_PLATFORM=linux/amd64
FROM --platform=${TARGET_PLATFORM} mcr.microsoft.com/dotnet/aspnet:8.0 AS runtime
WORKDIR /app

# Export the raw C++ build artifacts for the host to extract via 'docker cp'
COPY --from=build-env /src/Backend/CaseConversionAPI/CppLib/build /src/Backend/CaseConversionAPI/CppLib/build

# Note: No ENTRYPOINT defined as this container is utilized as an artifact source.