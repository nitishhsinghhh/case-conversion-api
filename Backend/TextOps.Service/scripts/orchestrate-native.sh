#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
# SYSTEM      : LexisEngine Infrastructure                          */
# COMPONENT   : orchestrate-native                                  */
# VERSION     : 1.0                                                 */
#                                                                   */
# DESCRIPTION : Configures, builds, and validates the LexisCore     */
#               engine using a unified matrix-compatible approach.  */
#*********************************************************************/

set -euo pipefail

#*********************************************************************/
# Logging Utilities                                                  */
#*********************************************************************/

RED='\033[0;31m'; 
GREEN='\033[0;32m'; 
YELLOW='\033[1;33m'; 
BLUE='\033[0;34m'; 
NC='\033[0m'

timestamp() { date +"%Y-%m-%d %H:%M:%S"; }
log_info() { echo -e "${BLUE}[$(timestamp)] [INFO]${NC} $1"; }
log_warn() { echo -e "${YELLOW}[$(timestamp)] [WARN]${NC} $1"; }
log_error() { echo -e "${RED}[$(timestamp)] [ERROR]${NC} $1"; }
log_success() { echo -e "${GREEN}[$(timestamp)] [SUCCESS]${NC} $1"; }

#*********************************************************************/
# 1. Environment & Path Resolution                                   */
#*********************************************************************/

log_info "Synchronizing workspace context..."

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
SERVICE_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

readonly CPP_ROOT="$SERVICE_ROOT/src/Lexis.Core"
readonly BUILD_DIR="$CPP_ROOT/build/macos-latest"
readonly TARGET_LIB="libLexisCore.dylib"

if [ ! -d "$CPP_ROOT" ]; then
    log_error "Target C++ core folder path missing: $CPP_ROOT"
    exit 1
fi

cd "$CPP_ROOT"

#*********************************************************************/
# 2. Metadata Purge Phase                                            */
#*********************************************************************/

log_info "Purging stale metadata and build cache entries..."
if [ -d "$CPP_ROOT/build" ]; then
    log_warn "Removing existing build cache directory: $CPP_ROOT/build"
    rm -rf "$CPP_ROOT/build"
fi

#*********************************************************************/
# 3. Build Phase                                                     */
#*********************************************************************/

log_info "===== Building Native Shared Library ====="
mkdir -p "$BUILD_DIR"

cmake -S "$CPP_ROOT" -B "$BUILD_DIR" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_OSX_ARCHITECTURES=arm64

NUM_CORES=$(sysctl -n hw.perflevel0.logicalcpu 2>/dev/null || sysctl -n hw.ncpu || echo 4)
cmake --build "$BUILD_DIR" --config Release --parallel "$NUM_CORES"

log_success "C++ Core built successfully."

#*********************************************************************/
# 4. Execution & Testing Layer                                       */
#*********************************************************************/

log_info "===== Executing Native Test Suites ====="

# Enable internal GoogleTest coloring automatically
export GTEST_COLOR=1

cd "$BUILD_DIR"

# Using ctest with standard output (GTest handles its own coloring)
if ctest --output-on-failure --verbose; then
    log_success "All native test suites passed successfully."
else
    log_error "One or more native test suites failed."
    exit 1
fi

#*********************************************************************/
# 5. Final Verification                                              */
#*********************************************************************/

if [ -f "$BUILD_DIR/$TARGET_LIB" ]; then
    log_success "Artifact ready at: $BUILD_DIR/$TARGET_LIB"
else
    log_error "Target artifact '$TARGET_LIB' was not created."
    exit 1
fi

cd "$SCRIPT_DIR"
log_info "Returned to scripts directory."