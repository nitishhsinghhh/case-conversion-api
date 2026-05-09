#!/bin/bash
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
#*********************************************************************/

set -e

# 1. Path Synchronization
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR/.."
echo "===== Context: Switched to $(pwd) ====="

# -----------------------------------------------------------------
# 2. Dependency Sync (Enhanced)
# -----------------------------------------------------------------
echo "===== Syncing Native Library for Test Environment ====="

SRC_LIB="CppLib/build/libProcessStringDLL.dylib"
TEST_BIN="tests/DotNetTests/bin/Release/net8.0"

mkdir -p "$TEST_BIN"

if [ -f "$SRC_LIB" ]; then
    # Use absolute paths to prevent resolution errors
    ABS_TEST_BIN="$(pwd)/$TEST_BIN"
    cp "$SRC_LIB" "$ABS_TEST_BIN/libProcessStringDLL.dylib"
    
    # Export the dynamic library path so dlopen finds it regardless of working directory
    export DYLD_LIBRARY_PATH="$ABS_TEST_BIN:$DYLD_LIBRARY_PATH"
    echo "Native library synced and DYLD_LIBRARY_PATH updated."
else
    echo "ERROR: libProcessStringDLL.dylib not found at $SRC_LIB"
    exit 1
fi

# -----------------------------------------------------------------
# 3. Execution (With explicit Architecture check)
# -----------------------------------------------------------------
echo "===== Verifying Architecture for M2 ====="
file "$TEST_BIN/libProcessStringDLL.dylib"

echo "===== Running .NET Tests ====="
# Removing --no-build to ensure the .NET side is fresh
dotnet test "$(pwd)/tests/DotNetTests/DotNetAPI.Tests.csproj" \
    -c Release \
    --no-restore \
    -v normal

# 4. Return to Project Root
cd "$SCRIPT_DIR/../../.."
echo "===== Script Finished at Root: $(pwd) ====="