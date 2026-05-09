#!/bin/bash
#*********************************************************************/
#  Utility Script - Local CLI App Runner (Temporary Context)         */
#  Version     : 1.2                                                 */
#                                                                    */
# Purpose   : Temporarily swaps CMakeLists to build/run the CLI app  */
#              without modifying the main project architecture.      */
# Location  : backend/CaseConversionAPI/CppLib/Scripts/run-local.sh  */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-04-16  Nitish Singh    Initial Swap Logic Script   */
# 1.1        2026-04-16  Nitish Singh    Added Absolute Path Trap    */
# 1.2        2026-05-09  Nitish Singh    Optimized for M2 P-Cores    */
#                                        and enhanced error cleanup. */
# 1.3       2026-05-16  Nitish Singh     Standardized logging,       */
#                                        UI consistency & sync logic.*/
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
CPP_ROOT="$SCRIPT_DIR/.."
cd "$CPP_ROOT"

if [ ! -f "CMakeListsLocalApp.txt" ]; then
    log_error "Critical file missing: CMakeListsLocalApp.txt not found in $CPP_ROOT"
    exit 1
fi

log_success "Workspace synchronized: $(pwd)"

#*********************************************************************/
# 2. Safety Backup & Context Swap                                    */
#*********************************************************************/

log_info "Swapping to Local App Configuration..."

# Create a backup of the original C++ library CMakeLists
cp CMakeLists.txt CMakeLists.txt.bak

# Overwrite with the standalone CLI App configuration
cp CMakeListsLocalApp.txt CMakeLists.txt

# --- Safety Trap ---
# Ensures the original CMakeLists.txt is restored regardless of success or failure
cleanup() {
    cp "$CPP_ROOT/CMakeLists.txt.bak" "$CPP_ROOT/CMakeLists.txt"
    rm "$CPP_ROOT/CMakeLists.txt.bak"
    log_success "Configuration context restored."
}
trap cleanup EXIT

#*********************************************************************/
# 3. Build Phase (M2 Optimization)                                   */
#*********************************************************************/
log_info "===== Building CLI Application ====="

mkdir -p build_local && cd build_local

# Detect available cores for max performance (M2 P-Cores / Linux nproc)
NUM_CORES=$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 1)

# Configure with high-performance flags
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_PCORES=ON
log_info "Utilizing $NUM_CORES cores for parallel compilation..."
cmake --build . --config Release --parallel "$NUM_CORES"

log_success "CLI App built successfully."

#*********************************************************************/
# 4. Execution Phase                                                 */
#*********************************************************************/
echo -e "\n===== Running CLI App (sourcecode.cpp) ====="

APP_BIN=""
for bin in "./app" "./app.exe" "./Release/app" "./Release/app.exe"; do
    if [ -f "$bin" ]; then APP_BIN="$bin"; break; fi
done

if [ -n "$APP_BIN" ]; then
    $APP_BIN
    log_success "Execution completed."
else
    log_error "Binary 'app' not found. Checking build directory:"
    ls -R
    exit 1
fi

#*********************************************************************/
# 5. Workspace Restoration                                           */
#*********************************************************************/
# Navigation back to root: backend/CaseConversionAPI/CppLib/Scripts/
cd ../../../..
log_info "Current project root: $(pwd)"
# Restoration of CMakeLists.txt is handled by the EXIT trap.