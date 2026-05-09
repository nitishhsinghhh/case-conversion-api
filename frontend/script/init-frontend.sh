#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
#  Utility Script - Frontend Workspace Initialization                */
#  Version     : 1.1                                                 */
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
#*********************************************************************/

set -e

# Root folder
PROJECT_NAME="string-conversion-ui"

echo "===== Initializing Frontend: $PROJECT_NAME ====="

# Create Directory Hierarchy
echo "Creating project structure..."
mkdir -p $PROJECT_NAME/src/api
mkdir -p $PROJECT_NAME/src/ui
mkdir -p $PROJECT_NAME/src/types

# Initialize Core Files
echo "Generating TypeScript source placeholders..."
touch $PROJECT_NAME/src/api/wordCaseApi.ts
touch $PROJECT_NAME/src/ui/converter.ts
touch $PROJECT_NAME/src/types/models.ts
touch $PROJECT_NAME/src/main.ts
touch $PROJECT_NAME/src/style.css

# Initialize Config Files
echo "Finalizing environment configuration..."
touch $PROJECT_NAME/index.html
touch $PROJECT_NAME/package.json
touch $PROJECT_NAME/tsconfig.json
touch $PROJECT_NAME/vite.config.ts

echo -e "\n===== Project structure created successfully! ====="
echo "Location: $(pwd)/$PROJECT_NAME"