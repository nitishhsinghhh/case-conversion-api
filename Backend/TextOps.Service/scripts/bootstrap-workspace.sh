#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
#  Utility Script - Project Scaffolding (TextOps.Service)            */
#  Version     : 1.0                                                 */
#                                                                    */
# Purpose   : Generates the monorepo hierarchy for C++ and .NET.     */
#             Aligned with M2 P-Core optimization & Lexis Engine.    */
# Location  : scripts/create_structure.sh                            */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-05-10  Nitish Singh    Initial scaffolding.        */
#*********************************************************************/

set -euo pipefail

# 1. Navigation Logic
# Get the absolute path to the 'scripts' directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# Move to the project root (TextOps.Service)
cd "$SCRIPT_DIR/.."

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
# 1. Root Initialization (Using current directory)                   */
#*********************************************************************/
# We use '.' to indicate the current working directory
ROOT_DIR="."
PROJECT_NAME="TextOps.Service"

log_info "===== Initializing Root Workspace: $PROJECT_NAME ====="

#*********************************************************************/
# 2. Native Layer (C++) - The Lexis Engine                           */
#*********************************************************************/
log_info "Creating C++ Engine structure (src/Lexis.Core)..."

mkdir -p "$ROOT_DIR/src/Lexis.Core/include/spellcheck"
mkdir -p "$ROOT_DIR/src/Lexis.Core/include/transform"
mkdir -p "$ROOT_DIR/src/Lexis.Core/include/interop"
mkdir -p "$ROOT_DIR/src/Lexis.Core/src"
mkdir -p "$ROOT_DIR/src/Lexis.Core/data"

touch "$ROOT_DIR/src/Lexis.Core/CMakeLists.txt"
touch "$ROOT_DIR/src/Lexis.Core/include/spellcheck/Trie.hpp"
touch "$ROOT_DIR/src/Lexis.Core/include/transform/CaseConverter.hpp"
touch "$ROOT_DIR/src/Lexis.Core/src/Export.cpp"

#*********************************************************************/
# 3. API Layer (.NET 8.0) - The Gateway                              */
#*********************************************************************/
log_info "Creating .NET API project structure (src/TextOps.Api)..."

mkdir -p "$ROOT_DIR/src/TextOps.Api/Controllers"
mkdir -p "$ROOT_DIR/src/TextOps.Api/Models"
mkdir -p "$ROOT_DIR/src/TextOps.Api/Services/Interop"
mkdir -p "$ROOT_DIR/src/TextOps.Api/Middleware"

cat <<EOF > "$ROOT_DIR/src/TextOps.Api/Program.cs"
// TextOps.Service - REST Gateway
var builder = WebApplication.CreateBuilder(args);
builder.Services.AddControllers();
var app = builder.Build();
app.MapControllers();
app.Run();
EOF

touch "$ROOT_DIR/src/TextOps.Api/Controllers/SpellCheckController.cs"
touch "$ROOT_DIR/src/TextOps.Api/Controllers/TransformController.cs"
touch "$ROOT_DIR/src/TextOps.Api/Services/Interop/NativeMethods.cs"

#*********************************************************************/
# 4. Infrastructure & Artifacts                                      */
#*********************************************************************/
log_info "Setting up artifacts and test directories..."

mkdir -p "$ROOT_DIR/artifacts/bin"
mkdir -p "$ROOT_DIR/tests/Lexis.Core.Tests"
mkdir -p "$ROOT_DIR/tests/TextOps.Api.Tests"
mkdir -p "$ROOT_DIR/scripts"

#*********************************************************************/
# 5. Finalizing Configuration                                        */
#*********************************************************************/
log_info "Finalizing configuration..."

if [ ! -f "$ROOT_DIR/README.md" ]; then
cat <<EOF > "$ROOT_DIR/README.md"
# $PROJECT_NAME

High-performance Text Intelligence & Word-as-a-Service Platform.
- **Lexis.Core**: C++20 Native Engine
- **TextOps.Api**: .NET 8 REST Gateway
EOF
fi

# Prevent script from copying itself if it's already in the destination
SCRIPT_DEST="$ROOT_DIR/scripts/create_structure.sh"
if [[ "$(realpath "$0")" != "$(realpath "$SCRIPT_DEST")" ]]; then
    cp "$0" "$SCRIPT_DEST"
fi

log_success "Root Scaffolding Complete."
log_info "Next Steps:"
log_info "1. cd src/TextOps.Api && dotnet new webapi --force"
log_info "2. Create a solution: dotnet new sln -n $PROJECT_NAME"