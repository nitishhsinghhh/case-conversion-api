#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
# SYSTEM      : CaseConversionAPI Infrastructure                     */
# SUBSYSTEM   : Native Processing Engine                             */
# COMPONENT   : orchestrate-native-docker                            */
# VERSION     : 1.3                                                  */
#                                                                    */
# DESCRIPTION : Coordinates local and containerized native build     */
#               pipelines to produce a unified cross-platform        */
#               binary distribution for the C++ conversion engine.   */
#                                                                    */
#               Orchestrates native macOS compilation, Linux         */
#               containerized validation, Windows cross-platform     */
#               artifact extraction, and consolidated runtime        */
#               synchronization for downstream consumers.            */
#                                                                    */
# FEATURES    :                                                      */
#               * Native macOS build orchestration                   */
#               * Linux containerized compilation                    */
#               * Windows cross-platform artifact generation         */
#               * Dockerized GoogleTest execution                    */
#               * Automated artifact extraction                      */
#               * Cross-platform binary synchronization              */
#               * Unified distribution directory generation          */
#               * Repository-aware path normalization                */
#                                                                    */
# SIDE EFFECTS: Creates Docker images and containers on the host.    */
#               Generates platform-specific shared libraries and     */
#               modifies build output directories during artifact    */
#               synchronization and cleanup operations.              */
#                                                                    */
# LOCATION    : Backend/CaseConversionAPI/CppLib/Scripts/            */
#               orchestrate-native-docker.sh                         */
#                                                                    */
# LICENSE     : Apache License, Version 2.0                          */
#               Licensed under the Apache License, Version 2.0.      */
#               You may obtain a copy of the License at              */
#               http://www.apache.org/licenses/LICENSE-2.0           */
#                                                                    */
# AUTHOR      : Nitish Singh (nitishhsinghhh)                        */
# CONTACT     : me.singhnitish@yandex.com                            */
#                                                                    */
# REVISION HISTORY:                                                  */
# ------------------------------------------------------------------ */
# Ver  Date        Author           Description                      */
# ---  ----------  --------------   -------------------------------- */
# 1.0  2026-05-14  Nitish Singh     Initial native orchestration     */
#                                   workflow with Docker artifact    */
#                                   extraction support.              */
# 1.1  2026-05-16  Nitish Singh     Added backend root path          */
#                                   normalization and improved       */
#                                   Docker context handling.         */
# 1.2  2026-05-20  Nitish Singh     Added containerized GoogleTest   */
#                                   execution and automated binary   */
#                                   synchronization pipeline.        */
# 1.3  2026-05-30  Nitish Singh     Infrastructure header            */
#                                   normalization and subsystem      */
#                                   classification alignment.        */
#*********************************************************************/

set -euo pipefail

# --- PATH DISCOVERY ---
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BACKEND_ROOT=$(realpath "$SCRIPT_DIR/../../../")
REPO_ROOT=$(realpath "$BACKEND_ROOT/..")

# Configuration
DOCKERFILE_PATH="$BACKEND_ROOT/CaseConversionAPI/CppLib/Scripts/Dockerfile"
DIST_DIR="$BACKEND_ROOT/CaseConversionAPI/CppLib/build"
NATIVE_SCRIPT_REL="Backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh"

log_info() { echo -e "\033[0;34m[$(date +'%T')] [INFO]\033[0m $1"; }
log_success() { echo -e "\033[0;32m[$(date +'%T')] [SUCCESS]\033[0m $1"; }
log_error() { echo -e "\033[0;31m[$(date +'%T')] [ERROR]\033[0m $1"; }

# Move to REPO_ROOT for Docker context consistency
cd "$REPO_ROOT"

# --- STEP 1: LOCAL MACOS BUILD ---
if [[ "$(uname)" == "Darwin" ]]; then
    log_info "MacOS detected. Running local native build & tests..."
    
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

if docker build \
    --progress=plain \
    --platform linux/amd64 \
    -t cpp-native-cross \
    -f "$DOCKERFILE_PATH" .; then

    # 1. Run Linux Tests
    log_info "Running C++ Core GoogleTest suite inside Linux Container Environment..."
    if docker run --rm cpp-native-cross /src/Backend/CaseConversionAPI/CppLib/build/ubuntu-latest/runTests; then
        log_success "Linux (Ubuntu) Container Core Tests Passed Successfully."
    else
        log_error "Linux Core Tests Failed within the container context."
        exit 1
    fi

    # 3. Extract and Sync Artifacts
    log_info "Extracting virtual artifacts from Docker..."
    CONTAINER_ID=$(docker create cpp-native-cross)
    TEMP_EXTRACT="$DIST_DIR/docker_temp"
    mkdir -p "$TEMP_EXTRACT"
    
    docker cp "$CONTAINER_ID:/src/Backend/CaseConversionAPI/CppLib/build/." "$TEMP_EXTRACT/"
    docker rm "$CONTAINER_ID"

    log_info "Syncing cross-platform target folders..."
    
    # Sync Linux
    if [ -d "$TEMP_EXTRACT/ubuntu-latest" ]; then
        mkdir -p "$DIST_DIR/ubuntu-latest"
        cp "$TEMP_EXTRACT/ubuntu-latest/libProcessStringDLL.so" "$DIST_DIR/ubuntu-latest/"
        log_success "Artifact Secured: ubuntu-latest/libProcessStringDLL.so"
    fi

    # Sync Windows
    if [ -d "$TEMP_EXTRACT/windows-latest" ]; then
        mkdir -p "$DIST_DIR/windows-latest"
        cp "$TEMP_EXTRACT/windows-latest/libProcessStringDLL.dll" "$DIST_DIR/windows-latest/"
        log_success "Artifact Secured: windows-latest/libProcessStringDLL.dll"
    fi

    # 4. Final Cleanup
    rm -rf "$TEMP_EXTRACT"
    docker rmi cpp-native-cross
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