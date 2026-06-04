#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
# SYSTEM      : CaseConversionAPI Infrastructure                     */
# SUBSYSTEM   : Repository Bootstrap Framework                       */
# COMPONENT   : create_structure                                     */
# VERSION     : 1.4                                                  */
#                                                                    */
# DESCRIPTION : Generates the foundational monorepo directory        */
#               hierarchy for the CaseConversionAPI platform.        */
#               Initializes native C++ components, .NET API          */
#               layers, test infrastructure, automation scripts,     */
#               and repository-level configuration assets.           */
#                                                                    */
# FEATURES    :                                                      */
#               * Native C++ project scaffolding                     */
#               * .NET API workspace initialization                  */
#               * Test framework directory generation                */
#               * Infrastructure and automation layout creation      */
#               * Root configuration and documentation setup         */
#               * Monorepo architecture standardization              */
#                                                                    */
# SIDE EFFECTS: Creates directories, generates boilerplate source    */
#               files, initializes repository configuration assets,  */
#               and copies automation scripts into the workspace.    */
#                                                                    */
# LICENSE     : Apache License, Version 2.0                          */
#               Licensed under the Apache License, Version 2.0.      */
#               You may obtain a copy of the License at              */
#               http://www.apache.org/licenses/LICENSE-2.0           */
#                                                                    */
# AUTHOR      : Nitish Singh (nitishhsinghhh)                        */
# CONTACT     : me.singhnitish@yandex.com                            */
#                                                                    */
# DEPENDENCIES:                                                      */
#               * Bash                                               */
#               * mkdir                                              */
#               * cp                                                 */
#               * cat                                                */
#               * touch                                              */
#                                                                    */
# LOCATION    : scripts/create_structure.sh                          */
#                                                                    */
# REVISION HISTORY:                                                  */
# ------------------------------------------------------------------ */
# Ver  Date        Author           Description                      */
# ---  ----------  --------------   -------------------------------- */
# 1.0  2026-04-14  Nitish Singh     Initial Structure Template       */
# 1.1  2026-04-16  Nitish Singh     Added Version Header             */
# 1.2  2026-05-09  Nitish Singh     Added M2 Alignment and           */
#                                    Explicit Versioning             */
# 1.3  2026-05-09  Nitish Singh     Standardized Logging, Headers,   */
#                                    and Cleanup Logic               */
# 1.4  2026-05-30  Nitish Singh     Refactored to Infrastructure     */
#                                    Metadata Standard               */
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
log_success() { echo -e "${GREEN}[$(timestamp)] [SUCCESS]${NC} $1"; }

#*********************************************************************/
# Dependency Validation                                              */
#*********************************************************************/

check_dependency() {
    local cmd="$1"

    if ! command -v "$cmd" >/dev/null 2>&1; then
        log_error "Missing dependency: $cmd"

        case "$cmd" in
            dotnet)
                echo "Install .NET SDK: https://dotnet.microsoft.com/download"
                ;;
            cmake)
                echo "Install CMake: https://cmake.org/download/"
                ;;
            git)
                echo "Install Git: https://git-scm.com/downloads"
                ;;
        esac

        exit 1
    fi
}

#*********************************************************************/
# 1. Root Initialization                                             */
#*********************************************************************/

ROOT="CaseConversionAPI"

log_info "===== Building Project Workspace: $ROOT ====="

# Ensure we are running from a clean slate or appending safely
mkdir -p "$ROOT"

#*********************************************************************/
# 2. Native Layer (C++) - Optimized for Interop                      */
#*********************************************************************/

log_info "Creating C++ library structure..."

mkdir -p "$ROOT/CppLib/include"
mkdir -p "$ROOT/CppLib/src"
mkdir -p "$ROOT/CppLib/Scripts"

# Placeholder for the native orchestration script
touch "$ROOT/CppLib/Scripts/run.sh"

#*********************************************************************/
# 3. API Layer (.NET 8.0+)                                           */
#*********************************************************************/

log_info "Creating .NET API project structure..."

mkdir -p "$ROOT/DotNetAPI/Controllers"
mkdir -p "$ROOT/DotNetAPI/Models"
mkdir -p "$ROOT/DotNetAPI/Services"
mkdir -p "$ROOT/DotNetAPI/Middleware"

# Creating standardized .NET boilerplate placeholders
cat <<EOF > "$ROOT/DotNetAPI/Program.cs"
// CaseConversionAPI - Entry Point
var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();
app.Run();
EOF

touch "$ROOT/DotNetAPI/DotNetAPI.csproj"
touch "$ROOT/DotNetAPI/Controllers/WordCaseController.cs"

#*********************************************************************/
# 4. Interop, Tests & Infrastructure                                 */
#*********************************************************************/
log_info "Creating Interop, Test, and DevOps directories..."

# lib: for compiled .so/.dll binaries
mkdir -p "$ROOT/lib"

# tests: split by domain
mkdir -p "$ROOT/tests/CppTests"
mkdir -p "$ROOT/tests/DotNetTests"

# scripts: root-level automation
mkdir -p "$ROOT/scripts"

#*********************************************************************/
# 5. Root Configuration Files                                        */
#*********************************************************************/
log_info "Finalizing root configuration..."

# Standard README
cat <<EOF > "$ROOT/README.md"
# $ROOT

High-performance Case Conversion API.
- **Native**: C++17 Core Engine
- **Managed**: .NET API Gateway
EOF

# Placeholder CMake for the root
touch "$ROOT/CMakeLists.txt"

# Ensure the current script is correctly placed in the new structure
cp "$0" "$ROOT/scripts/create_structure.sh" 2>/dev/null || true

log_success "Scaffolding Complete for $ROOT"
log_info "Next Steps: cd $ROOT && dotnet new sln"