#!/bin/bash
#*********************************************************************/
#  Master Native Orchestrator: C++ Core (The Big Three)              */
#  Version     : 1.0                                                 */
#                                                                    */
# Purpose   : Coordinates Local (macOS) and Dockerized (Linux/Win)   */
#             builds to generate a unified multi-platform binary set.*/
# Location  : backend/CaseConversionAPI/CppLib/Scripts/master-build.sh*/
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author           Description                */
# ------------------------------------------------------------------ */
# 1.0        2026-05-14  Nitish Singh     Initial Master Orchestrator*/
#                                         with Docker extraction.    */
#*********************************************************************/

set -euo pipefail

# --- PATH DISCOVERY ---
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BACKEND_ROOT=$(realpath "$SCRIPT_DIR/../../../")
REPO_ROOT=$(realpath "$BACKEND_ROOT/..")

# Configuration
DOCKERFILE_PATH="$BACKEND_ROOT/CaseConversionAPI/CppLib/Scripts/Dockerfile"
DIST_DIR="$BACKEND_ROOT/CaseConversionAPI/CppLib/build"
NATIVE_SCRIPT_REL="backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh"

log_info() { echo -e "\033[0;34m[$(date +'%T')] [INFO]\033[0m $1"; }
log_success() { echo -e "\033[0;32m[$(date +'%T')] [SUCCESS]\033[0m $1"; }
log_error() { echo -e "\033[0;31m[$(date +'%T')] [ERROR]\033[0m $1"; }

# Move to REPO_ROOT for Docker context consistency
cd "$REPO_ROOT"

# --- STEP 1: LOCAL MACOS BUILD ---
if [[ "$(uname)" == "Darwin" ]]; then
    log_info "MacOS detected. Running local native build & tests..."
    
    # Execute native script with the macos-latest target
    if bash "$NATIVE_SCRIPT_REL" macos-latest; then
        log_success "MacOS Local Build & Testing Complete."
    else
        log_error "MacOS Local Build failed. Check native logs."
        exit 1
    fi
else
    log_info "Non-MacOS environment. Skipping local macos-latest target."
fi

# --- STEP 2: DOCKER CROSS-BUILD (LINUX & WINDOWS) ---
log_info "Initiating Docker Build for Linux (.so) and Windows (.dll)..."

# Build the native-only Docker image
if docker build -t cpp-native-cross -f "$DOCKERFILE_PATH" .; then
    log_info "Extracting virtual artifacts from Docker..."
    
    CONTAINER_ID=$(docker create cpp-native-cross)
    TEMP_EXTRACT="$DIST_DIR/docker_temp"
    mkdir -p "$TEMP_EXTRACT"

    # Copy the internal build directory to local temp
    # The dot at the end ensures we copy contents of 'build'
    docker cp "$CONTAINER_ID:/src/backend/CaseConversionAPI/CppLib/build/." "$TEMP_EXTRACT/"

    # Cleanup Docker immediately
    docker rm "$CONTAINER_ID"
    docker rmi cpp-native-cross

    # --- STEP 3: SYNC VIRTUAL ARTIFACTS ---
    log_info "Syncing cross-platform target folders..."

    # Target: ubuntu-latest
    if [ -d "$TEMP_EXTRACT/ubuntu-latest" ]; then
        mkdir -p "$DIST_DIR/ubuntu-latest"
        cp "$TEMP_EXTRACT/ubuntu-latest/libProcessStringDLL.so" "$DIST_DIR/ubuntu-latest/"
        log_success "Artifact Secured: ubuntu-latest/libProcessStringDLL.so"
    fi

    # Target: windows-latest
    if [ -d "$TEMP_EXTRACT/windows-latest" ]; then
        mkdir -p "$DIST_DIR/windows-latest"
        cp "$TEMP_EXTRACT/windows-latest/libProcessStringDLL.dll" "$DIST_DIR/windows-latest/"
        log_success "Artifact Secured: windows-latest/libProcessStringDLL.dll"
    fi

    # Final Cleanup
    rm -rf "$TEMP_EXTRACT"
else
    log_error "Docker build failed. Check Dockerfile and native toolchains."
    exit 1
fi

# --- FINAL STATUS REPORT ---
echo -e "\n\033[1;35m--- Build Status Summary ---\033[0m"
[ -f "$DIST_DIR/macos-latest/libProcessStringDLL.dylib" ] && echo -e "  [✔] macos-latest   (.dylib)" || echo -e "  [ ] macos-latest   (Skipped/Missing)"
[ -f "$DIST_DIR/ubuntu-latest/libProcessStringDLL.so" ]    && echo -e "  [✔] ubuntu-latest  (.so)"    || echo -e "  [✘] ubuntu-latest  (Missing)"
[ -f "$DIST_DIR/windows-latest/libProcessStringDLL.dll" ]   && echo -e "  [✔] windows-latest (.dll)"   || echo -e "  [✘] windows-latest (Missing)"
echo -e "\033[1;35m----------------------------\033[0m"

log_info "All native artifacts are synchronized in: $DIST_DIR"