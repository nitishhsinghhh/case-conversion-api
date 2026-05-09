#!/bin/bash
#*********************************************************************/
#  Utility Script - C++ Core Orchestration (Monorepo)                */
#  Version     : 1.5                                                 */
#                                                                    */
# Purpose   : Configures, builds, and executes C++ logic & tests.    */
# Location  : backend/CaseConversionAPI/CppLib/Scripts/run.sh        */
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
# 1.5        2026-05-16  Nitish Singh    Integrated logging,         */
#                                        validation & structured UI. */
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

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
CPP_ROOT="$SCRIPT_DIR/.."
cd "$CPP_ROOT"
log_success "Workspace context set to: $(pwd)"

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

log_info "===== Configuring & Building C++ Core ($OS_TARGET) ====="

mkdir -p build && cd build

# Use the array expansion syntax to preserve argument boundaries
cmake .. "${CMAKE_ARGS[@]}"

# Detect available cores for max performance (M2 P-Cores / Linux nproc)
NUM_CORES=$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 1)
log_info "Utilizing $NUM_CORES cores for parallel build..."

cmake --build . --config Release --parallel "$NUM_CORES"
log_success "C++ Core built successfully."

#*********************************************************************/
# 5. Execution & Testing Layer                                       */
#*********************************************************************/

# Skip execution if cross-compiling for Windows on a non-Windows host
if [[ "$OS_TARGET" == *"windows"* ]] && [[ "$(uname)" != "Darwin" && "$(uname)" != *"MINGW"* && "$(uname)" != *"MSYS"* ]]; then
    log_warn "Windows binary built successfully. Skipping execution on current host."
else
    echo -e "\n===== Running Core Tests ====="
    
    TEST_BIN=""
    for bin in "./runTests" "./runTests.exe" "./Release/runTests.exe"; do
        if [ -f "$bin" ]; then TEST_BIN="$bin"; break; fi
    done

    if [ -n "$TEST_BIN" ]; then
        $TEST_BIN
        log_success "Tests completed successfully."
    else
        log_error "Test binary not found. Checking directory: $(pwd)"
        ls -R
        exit 1
    fi
fi

#*********************************************************************/
# 6. Workspace Restoration
#*********************************************************************/

# Navigation back to root: backend/CaseConversionAPI/CppLib/Scripts/

cd ../../../..

log_info "Restored to project root: $(pwd)"
log_success "C++ Orchestration Complete."