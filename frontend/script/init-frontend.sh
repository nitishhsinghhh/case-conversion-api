#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
#  Utility Script - Frontend Workspace Initialization                */
#  Version     : 1.2                                                 */
#                                                                    */
# Purpose   : Scaffolds the directory structure for the Vite/TS UI.  */
# Location  : scripts/init-frontend.sh                               */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-04-16  Nitish Singh    Initial Scaffold Script     */
# 1.1        2026-05-09  Nitish Singh    Added architecture sync and */
#                                        M2 project alignment.       */
# 1.2        2026-05-09  Nitish Singh    Standardized logging,       */
#                                        environment pre-checks.     */
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
log_warn() { echo -e "${YELLOW}[$(timestamp)] [WARN]${NC} $1"; }
log_error() { echo -e "${RED}[$(timestamp)] [ERROR]${NC} $1"; }
log_success() { echo -e "${GREEN}[$(timestamp)] [SUCCESS]${NC} $1"; }

#*********************************************************************/
# 1. Environment Pre-checks                                          */
#*********************************************************************/
log_info "Verifying development environment..."

if ! command -v node &> /dev/null; then
    log_error "Node.js is not installed. Please install it to proceed."
    exit 1
fi

#*********************************************************************/
# 2. Configuration & Hierarchy                                       */
#*********************************************************************/
PROJECT_NAME="string-conversion-ui"

log_info "===== Initializing Frontend: $PROJECT_NAME ====="

log_info "Creating project structure..."
mkdir -p "$PROJECT_NAME/src/api"
mkdir -p "$PROJECT_NAME/src/ui"
mkdir -p "$PROJECT_NAME/src/types"
mkdir -p "$PROJECT_NAME/src/assets"

#*********************************************************************/
# 3. Initialization of Source Assets                                 */
#*********************************************************************/
log_info "Generating TypeScript source placeholders..."

# Source files
touch "$PROJECT_NAME/src/api/wordCaseApi.ts"
touch "$PROJECT_NAME/src/ui/converter.ts"
touch "$PROJECT_NAME/src/types/models.ts"
touch "$PROJECT_NAME/src/main.ts"
touch "$PROJECT_NAME/src/style.css"

# Config files
log_info "Finalizing environment configuration..."
touch "$PROJECT_NAME/index.html"
touch "$PROJECT_NAME/package.json"
touch "$PROJECT_NAME/tsconfig.json"
touch "$PROJECT_NAME/vite.config.ts"

#*********************************************************************/
# 4. Finalization                                                    */
#*********************************************************************/
log_success "Project structure created successfully!"
log_info "Location: $(pwd)/$PROJECT_NAME"
log_info "Action Required: Run 'npm init -y' inside the folder to populate package.json."