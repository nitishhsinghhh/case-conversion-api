#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
#  Utility Script - .NET API Orchestration (Monorepo)                */
#  Version     : 1.6                                                 */
#                                                                    */
# Purpose   : Manages the lifecycle (Restore, Build, Publish, Run)   */
#              of the .NET 8 REST API within the monorepo context.   */
# Usage     : ./run-dotnet.sh                                        */
# Shell     : Bash (Targeted for Linux/macOS)                        */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-04-14  Nitish Singh    Initial .NET build script   */
# 1.1        2026-04-15  Nitish Singh    Added Dynamic Path Logic    */
# 1.2        2026-04-16  Nitish Singh    Refactored exit/root logic  */
# 1.3        2026-04-16  Nitish Singh    Added Native Dylib Sync     */
# 1.4        2026-04-16  Nitish Singh    Fixed naming prefix mismatch*/
# 1.5        2026-04-16  Nitish Singh    Added Port Cleanup logic    */
# 1.6        2026-05-09  Nitish Singh    Added M2 P-Core DYLD path   */
#                                        export and versioning.      */
#*********************************************************************/

# Exit immediately if any command fails
set -e

# -------------------------------
# 0. Port Cleanup
# -------------------------------
echo "===== Clearing Port 5000 ====="
# Finds the process ID on port 5000 and kills it; ignores errors if port is empty
lsof -ti:5000 | xargs kill -9 2>/dev/null || true

# -------------------------------
# 1. Path Synchronization
# -------------------------------
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR/../DotNetAPI"

echo "===== Context: Switched to $(pwd) ====="

# -------------------------------
# 2. Restore dependencies
# -------------------------------
echo -e "\n===== Restoring dependencies ====="
dotnet restore

# -------------------------------
# 3. Build the project
# -------------------------------
echo -e "\n===== Building project ====="
dotnet build -c Release

# -------------------------------
# 4. Sync Native Binaries
# -------------------------------
echo -e "\n===== Syncing Native C++ Library ====="
BIN_DIR="bin/Release/net8.0"
mkdir -p "$BIN_DIR"

# Try the most likely paths
POSSIBLE_PATHS=(
    "../CppLib/build/libProcessStringDLL.dylib"
    "../CppLib/build/lib/libProcessStringDLL.dylib"
)

SRC_LIB=""
for path in "${POSSIBLE_PATHS[@]}"; do
    if [ -f "$path" ]; then
        SRC_LIB="$path"
        break
    fi
done

if [ -z "$SRC_LIB" ]; then
    echo "ERROR: Native library not found. Checked: ${POSSIBLE_PATHS[*]}"
    exit 1
fi

# Use absolute path for DYLD resolution on macOS
ABS_BIN_DIR="$(pwd)/$BIN_DIR"

# Copy both with and without 'lib' prefix to satisfy P/Invoke across different environments
cp "$SRC_LIB" "$ABS_BIN_DIR/libProcessStringDLL.dylib"
cp "$SRC_LIB" "$ABS_BIN_DIR/ProcessStringDLL.dylib"

# Export library path for the current shell session to ensure the native engine is found
export DYLD_LIBRARY_PATH="$ABS_BIN_DIR:$DYLD_LIBRARY_PATH"

echo "Synced: $SRC_LIB -> $BIN_DIR/"
echo "Architecture Check:"
file "$ABS_BIN_DIR/libProcessStringDLL.dylib"

# -------------------------------
# 5. Publish the project
# -------------------------------
echo -e "\n===== Publishing project ====="
dotnet publish -c Release -o ../../../publish
cp "$ABS_BIN_DIR/libProcessStringDLL.dylib" ../../../publish/
cp "$ABS_BIN_DIR/ProcessStringDLL.dylib" ../../../publish/

# -------------------------------
# 6. Run the API
# -------------------------------
echo -e "\n===== Running API ====="
echo "===== API is LIVE. Press Ctrl+C to stop and return to root ====="

export ASPNETCORE_ENVIRONMENT=Development
dotnet run -c Release --no-build

# -------------------------------
# 7. Return to Monorepo Root
# -------------------------------
cd "$SCRIPT_DIR/../../.."
echo -e "\n===== Completed. Back in project root: $(pwd) ====="