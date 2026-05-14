#!/bin/bash
#*********************************************************************/
#  Utility Script - C++ Core Orchestration (Monorepo)                */
#  Version     : 1.5                                                 */
#                                                                    */
# Purpose   : Configures, builds, and executes C++ logic & tests.    */
# Location  : CppLib/Scripts/orchestrate-native.sh                   */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-04-14  Nitish Singh    Initial C++ Run Script      */
# 1.1        2026-04-15  Nitish Singh    Updated for /backend path   */
# 1.2        2026-04-16  Nitish Singh    Added Dynamic Path Sync     */
# 1.3        2026-05-02  Nitish Singh    Added Matrix OS Support     */
# 1.4        2026-05-09  Nitish Singh    Enhanced M2 P-Core detection*/
#                                        and script versioning.      */
# 1.5        2026-05-16  Nitish Singh    Standardized logging,       */
#                                        execution context validation*/
#                                        and dynamic path handling.  */
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
# 1. Argument Handling (Matrix OS)                                   */
#*********************************************************************/

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <matrix-os>"
  echo "Example: $0 macos-latest | ubuntu-latest | windows-latest"
  exit 1
fi

OS_TARGET=$(echo "$1" | tr '[:upper:]' '[:lower:]')
echo "Targeting Matrix OS: $OS_TARGET"

#*********************************************************************/
# 2. Environment & Path Synchronization                              */
#*********************************************************************/

log_info "Validating build environment..."

if ! command -v cmake >/dev/null 2>&1; then
    log_error "CMake is not installed. Please install CMake to proceed."
    exit 1
fi

# FIX: Define SCRIPT_DIR and CPP_ROOT before using them
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
CPP_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Define the structured build path
readonly BUILD_DIR="$CPP_ROOT/build/$OS_TARGET"

log_success "Workspace context set: $CPP_ROOT"
log_success "Build directory set: $BUILD_DIR"

#*********************************************************************/
# 3. OS-Specific Configuration                                       */
#*********************************************************************/

# Initialize CMAKE_ARGS as an array to handle spaces safely
CMAKE_ARGS=("-DCMAKE_BUILD_TYPE=Release")

case "$OS_TARGET" in
    *macos*)
        log_info "Configuring for macOS optimization..."
        CMAKE_ARGS+=("-DARCH_ARM64=ON" "-DUSE_PCORES=ON" "-DCMAKE_OSX_ARCHITECTURES=arm64")
        ;;
    *ubuntu*|*linux*)
        log_info "Configuring for Linux optimization..."
        CMAKE_ARGS+=("-DCMAKE_CXX_FLAGS=-O3 -march=native" "-G" "Unix Makefiles")
        ;;
    *windows*)
        if command -v x86_64-w64-mingw32-gcc &> /dev/null; then
            log_info "Configuring for MinGW Windows cross-compilation..."
            CMAKE_ARGS+=("-G" "Unix Makefiles")
            CMAKE_ARGS+=("-DCMAKE_SYSTEM_NAME=Windows")
            CMAKE_ARGS+=("-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc")
            CMAKE_ARGS+=("-DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++")
        else
            log_info "Configuring for MSVC Windows optimization..."
            CMAKE_ARGS+=("-G" "Visual Studio 17 2022" "-A" "x64")
        fi
        ;;
    *)
        log_warn "Unrecognized OS target. Using generic build flags."
        ;;
esac

#*********************************************************************/
# 4. Compilation Layer                                               */
#*********************************************************************/

log_info "===== Building C++ Core ($OS_TARGET) ====="

mkdir -p "$BUILD_DIR"

# FIX: Define NUM_CORES for parallel build performance
if [[ "$OS_TARGET" == *"macos"* ]]; then
    NUM_CORES=$(sysctl -n hw.perflevel0.logicalcpu 2>/dev/null || sysctl -n hw.ncpu || echo 4)
else
    NUM_CORES=$(nproc 2>/dev/null || echo 2)
fi

# Modern CMake build
cmake -S "$CPP_ROOT" -B "$BUILD_DIR" "${CMAKE_ARGS[@]}"

log_info "Utilizing $NUM_CORES cores for parallel build..."
cmake --build "$BUILD_DIR" --config Release --parallel "$NUM_CORES"

log_success "C++ Core built successfully."

#*********************************************************************/
# 5. Execution & Testing Layer                                       */
#*********************************************************************/

# Logic to determine if we can run the binary on the current host
CAN_RUN=false
[[ "$OS_TARGET" == *"macos"* && "$(uname)" == "Darwin" ]] && CAN_RUN=true
[[ "$OS_TARGET" == *"ubuntu"* && "$(uname)" == "Linux" ]] && CAN_RUN=true

if [ "$CAN_RUN" = true ]; then
    echo -e "\n===== Running Core Tests ====="
    
    # Search for test binary in the new structure
    TEST_BIN=$(find "$BUILD_DIR" -maxdepth 3 \( -name "runTests" -o -name "runTests.exe" \) | head -n 1)

    if [[ -n "$TEST_BIN" && -f "$TEST_BIN" ]]; then
        # Run from the binary's directory to ensure relative assets resolve
        cd "$(dirname "$TEST_BIN")"
        ./$(basename "$TEST_BIN")
        log_success "Tests completed."
    else
        log_error "Test binary not found in $BUILD_DIR."
        exit 1
    fi
else
    log_warn "Target $OS_TARGET is not native to host $(uname). Skipping execution."
fi

#*********************************************************************/
# 6. Workspace Restoration
#*********************************************************************/

cd "$SCRIPT_DIR"
log_info "Returned to scripts directory."