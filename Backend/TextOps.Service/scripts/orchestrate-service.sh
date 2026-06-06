#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*******************************************************************/
# SYSTEM      : TextOps.Service Infrastructure                     */
# SUBSYSTEM   : Managed API Runtime Layer (.NET 8)                 */
# COMPONENT   : run-dotnet                                         */
# VERSION     : 1.0                                                */
#                                                                  */
# DESCRIPTION : Orchestrates the complete lifecycle of the         */
#               TextOps.Api gateway, including dependency          */
#               restoration, compilation, native runtime           */
#               synchronization, and local execution.              */
#                                                                  */
#               Ensures ABI compatibility between the native C++   */
#               Lexis.Core engine and the .NET P/Invoke layer by   */
#               injecting the shared library into the runtime      */
#               context.                                           */
#                                                                  */
# FEATURES    :                                                    */
#               * Automated dependency restoration                 */
#               * Release-mode compilation                         */
#               * Native library discovery and synchronization     */
#               * Runtime environment preparation                  */
#               * Local API hosting and execution                  */
#                                                                  */
# SIDE EFFECTS: Terminates active listeners on port 5000.          */
#               Synchronizes native runtime binaries into          */
#               execution directories.                             */
#                                                                  */
# LICENSE     : Apache License, Version 2.0                        */
#               Licensed under the Apache License, Version 2.0.    */
#                                                                  */
# AUTHOR      : Nitish Singh (nitishhsinghhh)                      */
# CONTACT     : me.singhnitish@yandex.com                          */
#                                                                  */
# REVISION HISTORY:                                                */
# ---------------------------------------------------------------- */
# Ver  Date       Author         Description                       */
# ---  ---------- -------------- --------------------------------  */
# 1.0  2026-06-05 Nitish Singh   Initial .NET orchestration        */
#                                                                  */
#*******************************************************************/

set -euo pipefail

#*******************************************************************/
# Logging Utilities                                                */
#*******************************************************************/

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
# 1. Port Cleanup                                                   */
#*********************************************************************/

log_info "Clearing Port 5000..."
lsof -ti:5000 | xargs kill -9 2>/dev/null || true

#*********************************************************************/
# 2. Path Synchronization                                           */
#*********************************************************************/

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
API_DIR="$SCRIPT_DIR/../src/TextOps.Api"
CORE_BUILD_DIR="$SCRIPT_DIR/../src/Lexis.Core/build/macos-latest"

cd "$API_DIR"
log_success "Context: Switched to $(pwd)"

#*********************************************************************/
# 3. .NET Lifecycle (Restore & Build)                               */
#*********************************************************************/

log_info "Restoring .NET dependencies..."
dotnet restore

log_info "Building project (Release)..."
dotnet build -c Release --no-restore

#*********************************************************************/
# 4. Native Binary Synchronization                                  */
#*********************************************************************/

log_info "Syncing Native Lexis.Core Engine..."
LIB_NAME="libLexisCore.dylib"
BIN_DIR="bin/Release/net8.0"
mkdir -p "$BIN_DIR"

if [ -f "$CORE_BUILD_DIR/$LIB_NAME" ]; then
    cp "$CORE_BUILD_DIR/$LIB_NAME" "$BIN_DIR/"
    log_success "Native library synced: $LIB_NAME"
else
    log_error "Native library $LIB_NAME not found in $CORE_BUILD_DIR. Ensure C++ Core is built."
    exit 1
fi

#*********************************************************************/
# 5. Execution                                                      */
#*********************************************************************/

log_info "===== API is LIVE. Press Ctrl+C to stop ====="
export ASPNETCORE_ENVIRONMENT=Development

# Run without re-building to preserve the synced native library context
dotnet run -c Release --no-build

#*********************************************************************/
# 6. Workspace Restoration                                          */
#*********************************************************************/

cd "$SCRIPT_DIR/../.."
log_info "Restored to project root: $(pwd)"