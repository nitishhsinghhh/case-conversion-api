#!/bin/bash
#*********************************************************************/
#  Utility Script - Native DLL Context Runner (macOS Optimized)       */
#  Version     : 1.5                                                 */
#                                                                    */
# Purpose   : Standardized cache purge and native compilation for     */
#             the unified LexisEngine CMake project structure.        */
# Location  : Backend/TextOps.Service/scripts/                       */
#*********************************************************************/

set -euo pipefail

#*********************************************************************/
# Logging Utilities                                                  */
#*********************************************************************/
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

timestamp() { date +"%Y-%m-%d %H:%M:%S"; }
log_info() { echo -e "${BLUE}[$(timestamp)] [INFO]${NC} $1"; }
log_warn() { echo -e "${YELLOW}[$(timestamp)] [WARN]${NC} $1"; }
log_error() { echo -e "${RED}[$(timestamp)] [ERROR]${NC} $1"; }
log_success() { echo -e "${GREEN}[$(timestamp)] [SUCCESS]${NC} $1"; }

#*********************************************************************/
# 1. Environment & Path Explicit Resolution                           */
#*********************************************************************/
log_info "Synchronizing workspace context..."

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
SERVICE_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Explicitly map down into the C++ Engine sub-project folder layout
readonly CPP_ROOT="$SERVICE_ROOT/src/Lexis.Core"
readonly DOTNET_API_DIR="$SERVICE_ROOT/src/TextOps.Api"
readonly BUILD_DIR="$CPP_ROOT/build/dll-latest"

if [ ! -d "$CPP_ROOT" ]; then
    log_error "Target C++ core folder path missing: $CPP_ROOT"
    exit 1
fi

cd "$CPP_ROOT"

# Check for the unified standard file now instead of a separate DLL file
if [ ! -f "CMakeLists.txt" ]; then
    log_error "Critical configuration file missing: CMakeLists.txt not found in $(pwd)"
    exit 1
fi

log_success "Workspace synchronized at C++ Engine Root: $(pwd)"

#*********************************************************************/
# 2. Metadata Purge Phase (Wipe Stale CMake Cache/Build Context)      */
#*********************************************************************/
log_info "Purging stale metadata and build cache entries..."

if [ -d "$BUILD_DIR" ]; then
    log_warn "Removing existing build cache directory: $BUILD_DIR"
    rm -rf "$BUILD_DIR"
fi

rm -f "$CPP_ROOT/CMakeCache.txt"
rm -rf "$CPP_ROOT/CMakeFiles"

log_success "Cache purge complete. Workspace cleared."

#*********************************************************************/
# 3. Build Phase (M2 Optimization)                                   */
#*********************************************************************/
log_info "===== Building Native Shared Library ====="

mkdir -p "$BUILD_DIR"

# Target Apple Silicon Performance Cores for rapid parallel compilation
NUM_CORES=$(sysctl -n hw.perflevel0.logicalcpu 2>/dev/null || sysctl -n hw.ncpu || echo 2)

# Run standard generation against the unified CMakeLists.txt
cmake -S "$CPP_ROOT" -B "$BUILD_DIR" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_OSX_ARCHITECTURES=arm64

log_info "Utilizing $NUM_CORES processing cores for parallel compilation..."
cmake --build "$BUILD_DIR" --config Release --parallel "$NUM_CORES"

log_success "Shared library built successfully."

#*********************************************************************/
# 4. Deployment Phase (.NET Shared Binary Binding)                   */
#*********************************************************************/
echo -e "\n===== Deploying Shared Library (libLexisSpellCheckDLL.dylib) ====="

DYLIB_BIN=$(find "$BUILD_DIR" -maxdepth 2 -name "libLexisSpellCheckDLL.dylib" | head -n 1)

if [[ -n "$DYLIB_BIN" && -f "$DYLIB_BIN" ]]; then
    if [ -d "$DOTNET_API_DIR" ]; then
        log_info "Target deployment destination identified: $DOTNET_API_DIR"
        
        # Deploy straight into the .NET API project root
        cp "$DYLIB_BIN" "$DOTNET_API_DIR/"
        log_info "Copied artifact to API project folder."

        # Synchronize target binaries forward into active application build targets
        if [ -d "$DOTNET_API_DIR/bin/Debug/net8.0" ]; then
            cp "$DYLIB_BIN" "$DOTNET_API_DIR/bin/Debug/net8.0/"
            log_info "Synchronized binary to local bin/Debug bin target."
        fi
        if [ -d "$DOTNET_API_DIR/bin/Release/net8.0" ]; then
            cp "$DYLIB_BIN" "$DOTNET_API_DIR/bin/Release/net8.0/"
            log_info "Synchronized binary to local bin/Release bin target."
        fi
        
        log_success "Execution completed. Binary deployed successfully."
    else
        log_warn "Managed .NET project folder target '$DOTNET_API_DIR' was not located. Skipping deployment copy."
    fi
else
    log_error "Target artifact 'libLexisSpellCheckDLL.dylib' was not located in build outputs."
    exit 1
fi

#*********************************************************************/
# 5. Workspace Restoration                                           */
#*********************************************************************/
cd "$SCRIPT_DIR"
log_info "Returned to scripts directory."