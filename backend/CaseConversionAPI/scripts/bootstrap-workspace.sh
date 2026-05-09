#!/bin/bash
#*********************************************************************/
#  Utility Script - Project Scaffolding (Microsoft-Style)            */
#  Version     : 1.2                                                 */
#                                                                    */
# Purpose   : Generates the monorepo hierarchy for C++ and .NET.     */
# Location  : scripts/create_structure.sh                            */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-04-14  Nitish Singh    Initial Structure Template  */
# 1.1        2026-04-16  Nitish Singh    Added Version Header        */
# 1.2        2026-05-09  Nitish Singh    Explicit versioning and     */
#                                        M2 project alignment.       */
#*********************************************************************/

set -e

# Root folder
ROOT="CaseConversionAPI"

echo "===== Building Project Workspace: $ROOT ====="

# -----------------------------
# 1. Native Layer (C++)
# -----------------------------
echo "Creating C++ library structure..."
mkdir -p $ROOT/CppLib/include
mkdir -p $ROOT/CppLib/src

# -----------------------------
# 2. API Layer (.NET)
# -----------------------------
echo "Creating .NET API project structure..."
mkdir -p $ROOT/DotNetAPI/Controllers
mkdir -p $ROOT/DotNetAPI/Models
mkdir -p $ROOT/DotNetAPI/Services

# Placeholder .NET files
touch $ROOT/DotNetAPI/Program.cs
touch $ROOT/DotNetAPI/Startup.cs
touch $ROOT/DotNetAPI/DotNetAPI.csproj
touch $ROOT/DotNetAPI/Controllers/WordCaseController.cs

# -----------------------------
# 3. Interop & Test Assets
# -----------------------------
echo "Creating Interop and Test directories..."
mkdir -p $ROOT/lib
mkdir -p $ROOT/tests/CppTests
mkdir -p $ROOT/tests/DotNetTests

# -----------------------------
# 4. Tooling & Root Config
# -----------------------------
echo "Finalizing scripts and root configuration..."
mkdir -p $ROOT/scripts
touch $ROOT/scripts/create_structure.sh

touch $ROOT/README.md
touch $ROOT/CMakeLists.txt

echo "===== Scaffolding Complete for $ROOT ====="