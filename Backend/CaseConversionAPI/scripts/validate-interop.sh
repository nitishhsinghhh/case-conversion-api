#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
#  Utility Script - .NET Test Orchestration (Monorepo)               */
#  Version     : 1.2                                                 */
#                                                                    */
# Purpose   : Executes the .NET 8 Test Suite from the scripts folder.*/
# Usage     : ./run-tests.sh                                         */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-04-16  Nitish Singh    Initial Test Script         */
# 1.1        2026-04-30  Nitish Singh    Added Native Library Sync   */
# 1.2        2026-05-09  Nitish Singh    Enhanced DYLD path resolution
#                                        and M2 architecture checks. */
# 1.3        2026-05-09  Nitish Singh    Standardized logging,       */
#                                        cross-platform lib sync.    */
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
# 1. Path Synchronization                                            */
#*********************************************************************/

log_info "Synchronizing workspace context..."

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR/.."
log_success "Context: Switched to $(pwd)"

#*********************************************************************/
# 2. Dependency Sync (Cross-Platform)                                */
#*********************************************************************/

log_info "===== Syncing Native Library for Test Environment ====="

# Detect extension and expected build folder based on OS
LIB_EXT="so"
MATRIX_DIR="ubuntu-latest"

if [[ "$(uname)" == "Darwin" ]]; then
    LIB_EXT="dylib"
    MATRIX_DIR="macos"
elif [[ "$(uname)" == *"MINGW"* || "$(uname)" == *"MSYS"* ]]; then
    LIB_EXT="dll"
    MATRIX_DIR="windows"
fi

# Define potential source locations (Matrix subfolder vs standard flat path)
MATRIX_SRC_LIB="CppLib/build/${MATRIX_DIR}/libProcessStringDLL.$LIB_EXT"
FLAT_SRC_LIB="CppLib/build/libProcessStringDLL.$LIB_EXT"
TEST_BIN="tests/DotNetTests/bin/Release/net8.0"

mkdir -p "$TEST_BIN"
ABS_TEST_BIN="$(pwd)/$TEST_BIN"

# Resolve the correct source library location
if [ -f "$MATRIX_SRC_LIB" ]; then
    SRC_LIB="$MATRIX_SRC_LIB"
    log_info "Found native library in matrix folder: $SRC_LIB"
elif [ -f "$FLAT_SRC_LIB" ]; then
    SRC_LIB="$FLAT_SRC_LIB"
    log_info "Found native library in fallback folder: $SRC_LIB"
else
    log_error "Native library not found! Checked locations:"
    log_error "  1. $MATRIX_SRC_LIB"
    log_error "  2. $FLAT_SRC_LIB"
    log_error "Please build CppLib for your host target first."
    exit 1
fi

# Copy both standard and prefixed versions for P/Invoke flexibility
cp "$SRC_LIB" "$ABS_TEST_BIN/libProcessStringDLL.$LIB_EXT"
cp "$SRC_LIB" "$ABS_TEST_BIN/ProcessStringDLL.$LIB_EXT"

# Export paths for dynamic resolution
export DYLD_LIBRARY_PATH="$ABS_TEST_BIN:${DYLD_LIBRARY_PATH:-}"
export LD_LIBRARY_PATH="$ABS_TEST_BIN:${LD_LIBRARY_PATH:-}"

log_success "Native library synced ($LIB_EXT) and paths exported."

#*********************************************************************/
# 3. Execution & Validation                                          */
#*********************************************************************/

log_info "Verifying Binary Architecture..."
file "$ABS_TEST_BIN/libProcessStringDLL.$LIB_EXT"

log_info "===== Executing .NET Test Suite ====="

# Ensure a fresh build for tests without re-restoring dependencies
dotnet test "$(pwd)/tests/DotNetTests/DotNetAPI.Tests.csproj" \
    -c Release \
    --no-restore \
    -v normal

#*********************************************************************/
# 4. Workspace Restoration                                           */
#*********************************************************************/

cd "$SCRIPT_DIR/../../.."
log_info "Restored to project root: $(pwd)"
log_success "Test Orchestration Complete."