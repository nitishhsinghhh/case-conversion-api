#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
#  Utility Script - Frontend Scaffolding (Vite/TypeScript)           */
#  Version     : 1.2                                                 */
#                                                                    */
# Purpose   : Generates the directory hierarchy and boilerplate for  */
#              the Vite-based TypeScript frontend application.       */
# Location  : scripts/scaffold-frontend.sh                           */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date         Author           Description               */
# ------------------------------------------------------------------ */
# 1.0        2026-04-28   Nitish Singh     Initial UI Template       */
# 1.1        2026-05-09   Nitish Singh     Added explicit versioning */
#                                          to match monorepo style.  */
# 1.2        2026-05-09   Nitish Singh     Standardized logging,     */
#                                          boilerplate injection.    */
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
# 1. Configuration                                                   */
#*********************************************************************/
PROJECT_NAME="string-conversion-ui"

log_info "===== Building Frontend Workspace: $PROJECT_NAME ====="

#*********************************************************************/
# 2. Directory Hierarchy (Scalable Architecture)                     */
#*********************************************************************/
log_info "Creating source directories (API, Components, Types)..."

mkdir -p "$PROJECT_NAME/src/api"
mkdir -p "$PROJECT_NAME/src/components"
mkdir -p "$PROJECT_NAME/src/types"
mkdir -p "$PROJECT_NAME/src/styles"
mkdir -p "$PROJECT_NAME/public"

#*********************************************************************/
# 3. Component & Logic Assets with Boilerplate                       */
#*********************************************************************/
log_info "Generating TypeScript source placeholders..."

# Models/Types
cat <<EOF > "$PROJECT_NAME/src/types/models.ts"
export interface ConversionRequest {
    input: string;
}

export interface ConversionResponse {
    original: string;
    converted: string;
    processingTimeMs: number;
}
EOF

# API Client
touch "$PROJECT_NAME/src/api/wordCaseApi.ts"

# Main Entry Points
touch "$PROJECT_NAME/src/main.ts"
cat <<EOF > "$PROJECT_NAME/src/styles/style.css"
:root {
  font-family: Inter, system-ui, Avenir, Helvetica, Arial, sans-serif;
  background-color: #242424;
  color: white;
}
EOF

#*********************************************************************/
# 4. Tooling & Config Files                                          */
#*********************************************************************/
log_info "Finalizing environment configuration (Vite, TS, Node)..."

cat <<EOF > "$PROJECT_NAME/package.json"
{
  "name": "$PROJECT_NAME",
  "private": true,
  "version": "0.0.0",
  "type": "module",
  "scripts": {
    "dev": "vite",
    "build": "tsc && vite build",
    "preview": "vite preview"
  }
}
EOF

touch "$PROJECT_NAME/index.html"
touch "$PROJECT_NAME/tsconfig.json"
touch "$PROJECT_NAME/vite.config.ts"

#*********************************************************************/
# 5. Finalization                                                    */
#*********************************************************************/
log_success "Frontend structure created successfully!"
log_info "Next Steps: cd $PROJECT_NAME && npm install"