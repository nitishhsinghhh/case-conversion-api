#!/bin/bash

# SPDX-License-Identifier: Apache-2.0

######################################################################
# File        : build_and_test.sh
# Author      : Nitish Singh
# Created     : 2026-06-07
#
# Copyright (c) 2016-2026 Nitish Singh
# Licensed under the Apache License, Version 2.0
# See LICENSE file in project root for license information
#
# Module      : Build/Scripts
# Component   : Rust Case Conversion Engine
# Platform    : macOS / Linux
# Thread Safe : N/A
# API Status  : Stable
# Version     : 1.0.0
#
# Description : Automated build validation script for the Rust
#               Case Conversion Engine.
#
#               Performs:
#               1. Source formatting validation
#               2. Static analysis via Clippy
#               3. Integration and unit test execution
#               4. Release DLL/shared library build
#               5. Artifact verification
#
# Notes       : - Stops immediately on first failure
#             : - Intended for local development and CI usage
#             : - Produces cdylib artifacts for FFI consumers
#             : - Suitable for GitHub Actions integration
#
# Generated Artifacts:
#             : - librust_lib.dylib (macOS)
#             : - librust_lib.so    (Linux)
#             : - rust_lib.dll      (Windows)
#
# Revision History:
# --------------------------------------------------------------------
# Version    Date        Author          Description
# --------------------------------------------------------------------
# 1.0.0      2026-06-07  Nitish Singh    Initial implementation
######################################################################

set -e

echo "======================================"
echo " Rust Case Conversion Library Build"
echo "======================================"

echo ""
echo "[1/5] Formatting source..."
cargo fmt --all

echo ""
echo "[2/5] Running static analysis..."
cargo clippy --all-targets --all-features -- -D warnings

echo ""
echo "[3/5] Running tests..."
cargo test -- --nocapture

echo ""
echo "[4/5] Building release DLL..."
cargo build --release

echo ""
echo "[5/5] Listing generated artifacts..."
ls -lh ../target/release


echo "[6/5] Deploying artifact to .NET output directory..."

# Destination root
DEST_PATH="../../DotNetAPI/bin/Release/net8.0"

# 1. Check if the root .NET output directory exists
if [ ! -d "$DEST_PATH" ]; then
    echo "Error: Directory $DEST_PATH does not exist."
    exit 1
fi

# 2. Create the 'rust' subdirectory if it doesn't exist
# The -p flag ensures no error if it already exists and creates parents if needed
mkdir -p "$DEST_PATH/rust"

# 3. Perform the copy
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    cp "../target/release/librust_lib.dylib" "$DEST_PATH/rust/librust_lib.dylib"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    cp "../target/release/librust_lib.so" "$DEST_PATH/rust/librust_lib.so"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    # Windows
    cp "../target/release/rust_lib.dll" "$DEST_PATH/rust/librust_lib.dll"
else
    echo "Unsupported OS: $OSTYPE"
    exit 1
fi

echo "Deployment successful to $DEST_PATH/rust/"

echo ""
echo "======================================"
echo " Build Successful"
echo "======================================"