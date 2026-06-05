#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
# SYSTEM      : CaseConversionAPI Infrastructure                     */
# SUBSYSTEM   : Managed API Runtime Layer (.NET 8)                   */
# COMPONENT   : run-dotnet                                           */
# VERSION     : 1.8                                                  */
#                                                                    */
# DESCRIPTION : Orchestrates the complete lifecycle of the managed   */
#               API gateway, including dependency restoration,       */
#               compilation, native runtime synchronization,         */
#               publication, and local execution.                    */
#                                                                    */
#               Ensures ABI compatibility between the native C++     */
#               conversion engine and the .NET P/Invoke layer by     */
#               automatically resolving and injecting platform-      */
#               specific shared libraries into the runtime context.  */
#                                                                    */
# FEATURES    :                                                      */
#               * Automated dependency restoration                   */
#               * Release-mode compilation                           */
#               * Native library discovery and synchronization       */
#               * Dynamic platform detection                         */
#               * Publish artifact generation                        */
#               * Runtime environment preparation                    */
#               * Local API hosting and execution                    */
#                                                                    */
# SIDE EFFECTS: Terminates active listeners on port 5000.            */
#               Creates publish artifacts and synchronizes native    */
#               runtime binaries into execution directories.         */
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
# 1.0  2026-04-14  Nitish Singh     Initial .NET orchestration       */
#                                   workflow.                        */
# 1.1  2026-04-15  Nitish Singh     Added dynamic repository path    */
#                                   resolution.                      */
# 1.2  2026-04-16  Nitish Singh     Refactored execution flow and    */
#                                   root context handling.           */
# 1.3  2026-04-16  Nitish Singh     Added native library sync        */
#                                   automation.                      */
# 1.4  2026-04-16  Nitish Singh     Standardized native binary       */
#                                   naming conventions.              */
# 1.5  2026-04-16  Nitish Singh     Added port cleanup and runtime   */
#                                   recovery logic.                  */
# 1.6  2026-05-09  Nitish Singh     Added macOS dynamic linker       */
#                                   environment support.             */
# 1.7  2026-05-09  Nitish Singh     Standardized structured logging  */
#                                   and cross-platform library sync. */
# 1.8  2026-05-30  Nitish Singh     Enterprise header normalization, */
#                                   infrastructure metadata, and     */
#                                   lifecycle documentation.         */
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
# 1. Port Cleanup                                                    */
#*********************************************************************/

log_info "Clearing Port 5000..."
lsof -ti:5000 | xargs kill -9 2>/dev/null || true

#*********************************************************************/
# 2. Path Synchronization                                            */
#*********************************************************************/

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR/../DotNetAPI"
log_success "Context: Switched to $(pwd)"

#*********************************************************************/
# 3. .NET Lifecycle (Restore & Build)                                */
#*********************************************************************/

log_info "Restoring .NET dependencies..."
dotnet restore

log_info "Building project (Release)..."
dotnet build -c Release --no-restore

#*********************************************************************/
# 4. Native Binary Synchronization                                   */
#*********************************************************************/

log_info "Syncing Native C++ Engine..."
BIN_DIR="bin/Release/net8.0"
mkdir -p "$BIN_DIR"

LIB_EXT="so"
MATRIX_DIR="ubuntu-latest"

if [[ "$(uname)" == "Darwin" ]]; then
    LIB_EXT="dylib"
    MATRIX_DIR="macos"
elif [[ "$(uname)" == *"MINGW"* || "$(uname)" == *"MSYS"* ]]; then
    LIB_EXT="dll"
    MATRIX_DIR="windows"
fi

POSSIBLE_PATHS=(
    "../CppLib/build/${MATRIX_DIR}/libProcessStringDLL.$LIB_EXT"
    "../CppLib/build/libProcessStringDLL.$LIB_EXT"
    "../CppLib/build/lib/libProcessStringDLL.$LIB_EXT"
)

SRC_LIB=""
for path in "${POSSIBLE_PATHS[@]}"; do
    if [ -f "$path" ]; then SRC_LIB="$path"; break; fi
done

if [ -z "$SRC_LIB" ]; then
    log_error "Native library not found ($LIB_EXT). Ensure C++ Core is built."
    exit 1
fi

ABS_BIN_DIR="$(pwd)/$BIN_DIR"

# Copy library to ensure P/Invoke finds it in the execution context
cp "$SRC_LIB" "$ABS_BIN_DIR/libProcessStringDLL.$LIB_EXT"
cp "$SRC_LIB" "$ABS_BIN_DIR/ProcessStringDLL.$LIB_EXT"

# Export library path for native resolution (macOS/Linux)
export DYLD_LIBRARY_PATH="$ABS_BIN_DIR:${DYLD_LIBRARY_PATH:-}"
export LD_LIBRARY_PATH="$ABS_BIN_DIR:${LD_LIBRARY_PATH:-}"

log_success "Native library synced and paths exported."
file "$ABS_BIN_DIR/libProcessStringDLL.$LIB_EXT"

#*********************************************************************/
# 5. Publish & Execution                                             */
#*********************************************************************/

log_info "Publishing API artifacts..."
PUBLISH_DIR="../../../publish"
dotnet publish -c Release -o "$PUBLISH_DIR" --no-build

# Ensure native binaries are in the publish folder
cp "$ABS_BIN_DIR/"*.$LIB_EXT "$PUBLISH_DIR/"

log_info "===== API is LIVE. Press Ctrl+C to stop ====="
export ASPNETCORE_ENVIRONMENT=Development

# Run without re-building to preserve the synced native library context
dotnet run -c Release --no-build

#*********************************************************************/
# 6. Workspace Restoration                                           */
#*********************************************************************/

cd "$SCRIPT_DIR/../../.."
log_info "Restored to project root: $(pwd)"