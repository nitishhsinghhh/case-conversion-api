#!/bin/bash
#*********************************************************************/
#  Utility Script - Local CLI App Runner (Temporary Context)          */
#  Version     : 1.2                                                 */
#                                                                    */
# Purpose   : Temporarily swaps CMakeLists to build/run the CLI app   */
#              without modifying the main project architecture.      */
# Location  : backend/CaseConversionAPI/CppLib/Scripts/               */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-04-16  Nitish Singh    Initial Swap Logic Script   */
# 1.1        2026-04-16  Nitish Singh    Added Absolute Path Trap    */
# 1.2        2026-05-09  Nitish Singh    Optimized for M2 P-Cores    */
#                                        and enhanced error cleanup. */
#*********************************************************************/

set -e

# 1. Path Synchronization
# Ensures the script can be executed from any directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
CPP_ROOT="$SCRIPT_DIR/.."
cd "$CPP_ROOT"

# 2. Safety Backup & Swap
echo "===== Swapping to Local App Configuration ====="
if [ ! -f "CMakeListsLocalApp.txt" ]; then
    echo "ERROR: CMakeListsLocalApp.txt not found in $CPP_ROOT"
    exit 1
fi

# Create a backup of the DLL-based CMakeLists
cp CMakeLists.txt CMakeLists.txt.bak
# Overwrite with the Local App version
cp CMakeListsLocalApp.txt CMakeLists.txt

# 3. Enhanced Trap Logic
# The 'EXIT' trap runs regardless of success, failure, or Ctrl+C.
# Uses absolute paths ($CPP_ROOT) to ensure restoration works even
# if the script ends while inside the 'build_local' directory.
trap 'cp "$CPP_ROOT/CMakeLists.txt.bak" "$CPP_ROOT/CMakeLists.txt" && rm "$CPP_ROOT/CMakeLists.txt.bak"; echo "===== Configuration Restored ====="' EXIT

# 4. Build Phase
echo "===== Building CLI App ====="
mkdir -p build_local
cd build_local

# Identify hardware for parallel compilation (M2 Optimization)
NUM_CORES=$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 1)

# Configure and Build using the temporary CMakeLists.txt
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_PCORES=ON
cmake --build . --config Release --parallel "$NUM_CORES"

# 5. Execution Phase
echo -e "\n===== Running CLI App (sourcecode.cpp) ====="
# Run the 'app' binary produced by CMakeListsLocalApp.txt
if [ -f "./app" ]; then
    ./app
elif [ -f "./Release/app" ]; then
    ./Release/app
else
    echo "Error: 'app' binary not found in build_local directory."
    exit 1
fi

# 6. Workspace Restoration
# Restoration is handled automatically by the 'trap' defined in Section 3.