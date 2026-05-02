#!/bin/bash
#*********************************************************************/
#  Utility Script - C++ Core Orchestration (Monorepo)                 */
#                                                                    */
# Purpose   : Configures, builds, and executes C++ logic & tests.     */
# Location  : backend/CaseConversionAPI/CppLib/Scripts/run.sh         */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-04-14  Nitish Singh    Initial C++ Run Script      */
# 1.1        2026-04-15  Nitish Singh    Updated for /backend path   */
# 1.2        2026-04-16  Nitish Singh    Added Dynamic Path Sync     */
# 1.3        2026-05-02  Nitish Singh    Added Matrix OS Support     */
#*********************************************************************/

set -euo pipefail

# -------------------------------
# 1. Argument Handling (Matrix OS)
# -------------------------------
if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <matrix-os>"
  echo "Example: $0 macos-latest | ubuntu-latest | windows-latest"
  exit 1
fi

OS_TARGET=$(echo "$1" | tr '[:upper:]' '[:lower:]')
echo "Targeting Matrix OS: $OS_TARGET"

# -------------------------------
# 2. Path Synchronization
# -------------------------------
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
CPP_ROOT="$SCRIPT_DIR/.."
cd "$CPP_ROOT"

# -------------------------------
# 3. OS-Specific Configuration
# -------------------------------
# Initialize CMAKE_ARGS as an array to handle spaces safely
CMAKE_ARGS=("-DCMAKE_BUILD_TYPE=Release")

if [[ "$OS_TARGET" == *"macos"* ]]; then
  echo "Setting up Apple Silicon / macOS optimization..."
  CMAKE_ARGS+=("-DARCH_ARM64=ON" "-DUSE_PCORES=ON")
  
elif [[ "$OS_TARGET" == *"ubuntu"* || "$OS_TARGET" == *"linux"* ]]; then
  echo "Setting up Linux / GCC optimization..."
  CMAKE_ARGS+=("-DCMAKE_CXX_FLAGS=-O3 -march=native" "-G" "Unix Makefiles")

elif [[ "$OS_TARGET" == *"windows"* ]]; then
  # Check if we are inside the Docker container (where MinGW lives)
  if command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo "Setting up MinGW / Windows cross-compilation..."
    CMAKE_ARGS+=("-G" "Unix Makefiles")
    CMAKE_ARGS+=("-DCMAKE_SYSTEM_NAME=Windows")
    CMAKE_ARGS+=("-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc")
    CMAKE_ARGS+=("-DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++")
  else
    echo "Setting up MSVC / Windows optimization..."
    CMAKE_ARGS+=("-G" "Visual Studio 17 2022" "-A" "x64")
  fi  

else
  echo "Warning: Unrecognized OS target. Using generic build flags."
fi 
# -------------------------------
# 4. Compilation Layer
# -------------------------------
echo "===== Configuring & Building C++ Core ($OS_TARGET) ====="

mkdir -p build
cd build

# Use the array expansion syntax to preserve argument boundaries
cmake .. "${CMAKE_ARGS[@]}"

# Detect available cores for max performance
NUM_CORES=$(nproc 2>/dev/null || sysctl -n hw.ncpu || echo 1)
echo "Utilizing $NUM_CORES cores for parallel build..."

cmake --build . --config Release --parallel "$NUM_CORES"

# -------------------------------
# 5. Execution Layer
# -------------------------------
# Skip execution if we are cross-compiling Windows binaries on a Linux host
if [[ "$OS_TARGET" == *"windows"* ]] && [[ "$(uname)" == "Linux" ]]; then
    echo -e "\n[Notice] Windows binary built successfully. Skipping execution on Linux host."
else
    echo -e "\n===== Running Core Tests ====="
    
    if [ -f "./runTests" ]; then
        ./runTests
    elif [ -f "./runTests.exe" ]; then
        ./runTests.exe
    elif [ -f "./Release/runTests.exe" ]; then
        ./Release/runTests.exe
    else
        echo "Error: Test binary not found. Checking directory: $(pwd)"
        ls -R
        exit 1
    fi
fi

# -------------------------------
# 6. Workspace Restoration
# -------------------------------
cd ../../../..

echo -e "\n===== Completed. Back in project root: $(pwd) ====="