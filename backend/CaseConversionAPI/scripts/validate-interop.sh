#!/bin/bash
#*********************************************************************/
#  Utility Script - .NET Test Orchestration (Monorepo)               */
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
#*********************************************************************/

set -e

# 1. Path Synchronization
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR/.."
echo "===== Context: Switched to $(pwd) ====="

# -----------------------------------------------------------------
# 2. Dependency Sync (The Fix)
# -----------------------------------------------------------------
echo "===== Syncing Native Library for Test Environment ====="

# Source of your built C++ Library
SRC_LIB="CppLib/build/libProcessStringDLL.dylib"
# Target bin where .NET runs Release tests
TEST_BIN="tests/DotNetTests/bin/Release/net8.0"

# Create the directory if it doesn't exist
mkdir -p "$TEST_BIN"

if [ -f "$SRC_LIB" ]; then
    # Copy both naming conventions to satisfy macOS P/Invoke
    cp "$SRC_LIB" "$TEST_BIN/libProcessStringDLL.dylib"
    cp "$SRC_LIB" "$TEST_BIN/ProcessStringDLL.dylib"
    echo "Native library synced to $TEST_BIN"
else
    echo "ERROR: libProcessStringDLL.dylib not found. Build C++ project first!"
    exit 1
fi

# -----------------------------------------------------------------
# 3. Execution
# -----------------------------------------------------------------
echo "===== Running .NET Tests ====="

# Use the full path to the project file to avoid context ambiguity
# Also, removing --no-build temporarily can help verify if it's a sync issue
dotnet test "$(pwd)/tests/DotNetTests/DotNetAPI.Tests.csproj" \
    -c Release \
    --no-restore \
    -v normal

echo "===== Tests completed ====="

# 4. Return to Project Root
cd "$SCRIPT_DIR/../../.."
echo "===== Script Finished at Root: $(pwd) ====="