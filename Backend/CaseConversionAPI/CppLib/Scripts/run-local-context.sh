#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
# SYSTEM      : CaseConversionAPI Infrastructure                     */
# SUBSYSTEM   : Native Processing Engine                             */
# COMPONENT   : run-local                                            */
# VERSION     : 1.5                                                  */
#                                                                    */
# DESCRIPTION : Builds and executes the standalone native CLI        */
#               application without altering the primary library     */
#               architecture or repository workflow.                 */
#                                                                    */
#               Performs temporary build-context substitution,       */
#               automated source formatting, optimized Apple         */
#               Silicon compilation, and controlled restoration      */
#               of the native project configuration.                 */
#                                                                    */
# FEATURES    :                                                      */
#               * Dynamic workspace synchronization                  */
#               * Safe CMakeLists backup and restore workflow        */
#               * Apple Silicon (M-Series) optimization             */
#               * Automated clang-format integration                 */
#               * Parallelized multi-core compilation                */
#               * Execution context validation                       */
#               * Automated cleanup and restoration                  */
#               * Standalone CLI application execution               */
#                                                                    */
# SIDE EFFECTS: Temporarily replaces the active CMakeLists.txt       */
#               configuration during execution. Generates build      */
#               artifacts and modifies local build directories.      */
#                                                                    */
# LOCATION    : Backend/CaseConversionAPI/CppLib/Scripts/            */
#               run-local.sh                                         */
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
# 1.0  2026-04-16  Nitish Singh     Initial local application        */
#                                   runner implementation.           */
# 1.1  2026-04-16  Nitish Singh     Added absolute path handling     */
#                                   and restoration safeguards.      */
# 1.2  2026-05-09  Nitish Singh     Added Apple Silicon              */
#                                   optimizations and cleanup.       */
# 1.3  2026-05-16  Nitish Singh     Added execution context          */
#                                   validation and path sync.        */
# 1.4  2026-05-28  Nitish Singh     Added automated clang-format     */
#                                   execution and fallback logic.    */
# 1.5  2026-05-30  Nitish Singh     Infrastructure header            */
#                                   normalization and subsystem      */
#                                   classification alignment.        */
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
# 1. Environment & Path Synchronization                              */
#*********************************************************************/

log_info "Synchronizing workspace context..."

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
CPP_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$CPP_ROOT"

if [ ! -f "CMakeListsLocalApp.txt" ]; then
    log_error "Critical file missing: CMakeListsLocalApp.txt not found in $CPP_ROOT"
    exit 1
fi

readonly BUILD_DIR="$CPP_ROOT/build/macos-latest"

log_success "Workspace synchronized: $(pwd)"

#*********************************************************************/
# 2. Safety Backup & Context Swap                                    */
#*********************************************************************/

log_info "Swapping to Local App Configuration..."

# Create a backup of the original C++ library CMakeLists
cp CMakeLists.txt CMakeLists.txt.bak

# Overwrite with the standalone CLI App configuration
cp CMakeListsLocalApp.txt CMakeLists.txt

cleanup() {
    if [ -f "$CPP_ROOT/CMakeLists.txt.bak" ]; then
        mv "$CPP_ROOT/CMakeLists.txt.bak" "$CPP_ROOT/CMakeLists.txt"
        log_success "Configuration context restored."
    fi
}
trap cleanup EXIT

#*********************************************************************/
# 3. Build Phase (M2 Optimization)                                   */
#*********************************************************************/

log_info "===== Building CLI Application ====="

mkdir -p "$BUILD_DIR"

# Corrected M2 P-Core detection
NUM_CORES=$(sysctl -n hw.perflevel0.logicalcpu 2>/dev/null || sysctl -n hw.ncpu || echo 2)

# FIX: Changed -arch arm6 to -arch arm64
cmake -S "$CPP_ROOT" -B "$BUILD_DIR" \
      -DCMAKE_BUILD_TYPE=Release \
      -DARCH_ARM64=ON \
      -DUSE_PCORES=ON \
      -DCMAKE_OSX_ARCHITECTURES=arm64

# Temporarily disable strict exit-on-error for the formatter target
set +e
log_info "Executing source workspace auto-formatting via clang-format..."
cmake --build "$BUILD_DIR" --target format
if [ $? -eq 0 ]; then
    log_success "Workspace formatting complete."
else
    log_warn "Formatting target encountered an initialization anomaly; proceeding to compilation."
fi
set -e # Re-enable strict error tracking for compilation safety

log_info "Utilizing $NUM_CORES cores for parallel compilation..."
cmake --build "$BUILD_DIR" --config Release --parallel "$NUM_CORES"

log_success "CLI App built successfully."

#*********************************************************************/
# 4. Execution Phase                                                 */
#*********************************************************************/

echo -e "\n===== Running CLI App (sourcecode.cpp) ====="

APP_BIN=$(find "$BUILD_DIR" -maxdepth 2 \( -name "app" -o -name "app.exe" \) | head -n 1)

if [[ -n "$APP_BIN" && -f "$APP_BIN" ]]; then
    cd "$(dirname "$APP_BIN")"
    ./$(basename "$APP_BIN")
    log_success "Execution completed."
else
    log_error "Binary 'app' not found."
    exit 1
fi

#*********************************************************************/
# 5. Workspace Restoration                                           */
#*********************************************************************/

# The trap handles the CMakeLists restoration.
cd "$SCRIPT_DIR"
log_info "Returned to scripts directory."