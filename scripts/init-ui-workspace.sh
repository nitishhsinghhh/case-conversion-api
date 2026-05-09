#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
#  Utility Script - Frontend Scaffolding (Vite/TypeScript)           */
#  Version     : 1.1                                                 */
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
#*********************************************************************/

# Exit immediately if a command fails
set -e

# -------------------------------
# 1. Configuration
# -------------------------------
PROJECT_NAME="string-conversion-ui"

echo "===== Building Frontend Workspace: $PROJECT_NAME ====="

# -------------------------------
# 2. Directory Hierarchy
# -------------------------------
echo "Creating source directories (API, UI, Types)..."
mkdir -p $PROJECT_NAME/src/api
mkdir -p $PROJECT_NAME/src/ui
mkdir -p $PROJECT_NAME/src/types

# -------------------------------
# 3. Component & Logic Assets
# -------------------------------
echo "Generating TypeScript source placeholders..."
touch $PROJECT_NAME/src/api/wordCaseApi.ts
touch $PROJECT_NAME/src/ui/converter.ts
touch $PROJECT_NAME/src/types/models.ts
touch $PROJECT_NAME/src/main.ts
touch $PROJECT_NAME/src/style.css

# -------------------------------
# 4. Tooling & Config Files
# -------------------------------
echo "Finalizing environment configuration (Vite, TS, Node)..."
touch $PROJECT_NAME/index.html
touch $PROJECT_NAME/package.json
touch $PROJECT_NAME/tsconfig.json
touch $PROJECT_NAME/vite.config.ts

echo -e "\n===== Project structure created successfully! ====="