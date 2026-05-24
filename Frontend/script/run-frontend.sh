#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
#  Utility Script - Frontend Development Server                      */
#  Version     : 1.3                                                 */
#                                                                    */
# Purpose   : Manages the lifecycle and execution of the Vite        */
#              development server for the TypeScript UI.             */
# Location  : scripts/run-frontend.sh                                */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-04-16  Nitish Singh    Initial Dev Runner          */
# 1.1        2026-04-16  Nitish Singh    Added Apple Silicon Fix     */
# 1.2        2026-05-06  Nitish Singh    Added Binary Path Validation*/
# 1.3        2026-05-09  Nitish Singh    Standardized versioning and */
#                                        M2 dev optimization.        */
#*********************************************************************/

set -e

# Path Synchronization
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
FRONTEND_ROOT="$SCRIPT_DIR/.."
cd "$FRONTEND_ROOT"

echo "===== Starting Frontend Environment ====="

# Ensure dependencies are installed
if [ ! -d "node_modules" ]; then
    echo "node_modules not found. Installing arm64 native bindings for M2..."
    npm install
fi

# Define the local Vite path to avoid "command not found" issues
VITE_BIN="./node_modules/.bin/vite"

# Final check: If Vite binary is still missing, force an install
if [ ! -f "$VITE_BIN" ]; then
    echo "Vite binary missing from node_modules. Running npm install..."
    npm install
fi

# Run Vite dev server using the explicit local path
# Forcing HMR (Hot Module Replacement) to stay responsive on M2
echo "Launching Vite server on port 5175..."
$VITE_BIN --port 5175 --open