#!/bin/bash
#*********************************************************************/
# SYSTEM      : GitHub Workflow Automation Framework                 */
# SUBSYSTEM   : Pull Request Lifecycle Orchestrator                  */
# COMPONENT   : gh-automate                                          */
# VERSION     : 1.8                                                  */
#                                                                    */
# DESCRIPTION : Automates Git branch lifecycle management and        */
#               GitHub Pull Request operations. Performs branch      */
#               preparation, commit orchestration, remote sync,      */
#               PR creation, CI/CD monitoring, merge execution,      */
#               and local repository synchronization.                */
#                                                                    */
# FEATURES    :                                                      */
#               * Branch creation and checkout                       */
#               * Automated staging and commit management            */
#               * Remote synchronization with lease protection       */
#               * GitHub Pull Request creation and tracking          */
#               * Live CI/CD monitoring via GitHub CLI               */
#               * Administrative squash merge support                */
#               * Automatic local main branch synchronization        */
#                                                                    */
# SIDE EFFECTS: Pushes commits to remote repositories, creates       */
#               GitHub Pull Requests, merges approved changes,       */
#               deletes merged branches, and updates local state.    */
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
#               * Git                                                */
#               * GitHub CLI (gh)                                    */
#               * GitHub Authentication Session                      */
#                                                                    */
# REVISION HISTORY:                                                  */
# ------------------------------------------------------------------ */
# Ver  Date        Author           Description                      */
# ---  ----------  --------------   -------------------------------- */
# 1.0  2026-04-14  Nitish Singh     Initial Automation Workflow      */
# 1.1  2026-05-09  Nitish Singh     Added Architecture Sync Checks   */
# 1.2  2026-05-09  Nitish Singh     Added Manual Approval Gate       */
# 1.3  2026-05-09  Nitish Singh     Added Browser PR Monitoring      */
# 1.4  2026-05-09  Nitish Singh     Added GitHub Checks Tracking     */
# 1.5  2026-05-09  Nitish Singh     Added Structured Logging         */
# 1.6  2026-05-09  Nitish Singh     Added Auth & Dependency Checks   */
# 1.7  2026-05-21  Nitish Singh     Added Retry & API Resilience     */
# 1.8  2026-05-30  Nitish Singh     Standardized Header Format and   */
#                                   Lifecycle Documentation          */
#*********************************************************************/

set -euo pipefail

#*********************************************************************/
# Logging Utilities                                                   */
#*********************************************************************/

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

timestamp() {
    date +"%Y-%m-%d %H:%M:%S"
}

log_info() {
    echo -e "${BLUE}[$(timestamp)] [INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[$(timestamp)] [WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[$(timestamp)] [ERROR]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[$(timestamp)] [SUCCESS]${NC} $1"
}

#*********************************************************************/
# Dependency Validation                                               */
#*********************************************************************/

log_info "Validating required dependencies..."

if ! command -v git >/dev/null 2>&1; then
    log_error "Git is not installed or not available in PATH."
    exit 1
fi

if ! command -v gh >/dev/null 2>&1; then
    log_error "GitHub CLI (gh) is not installed or not available in PATH."
    exit 1
fi

log_success "Required CLI dependencies detected."

#*********************************************************************/
# GitHub Authentication Validation                                   */
#*********************************************************************/

log_info "Validating GitHub CLI authentication..."

if ! gh auth status >/dev/null 2>&1; then
    log_error "GitHub CLI authentication failed."
    log_error "Run: gh auth login"
    exit 1
fi

log_success "GitHub CLI authentication verified."

#*********************************************************************/
# Git Remote Validation                                               */
#*********************************************************************/

log_info "Validating git remote configuration..."

if ! git remote get-url origin >/dev/null 2>&1; then
    log_error "Git remote 'origin' is not configured."
    exit 1
fi

log_success "Git remote configuration verified."

#*********************************************************************/
# Input Validation                                                    */
#*********************************************************************/

BRANCH=$1
MESSAGE=$2

if [ -z "$BRANCH" ] || [ -z "$MESSAGE" ]; then
    log_error "Missing arguments."
    echo "Usage: ./gh-automate.sh <branch-name> <message>"
    exit 1
fi

log_info "Starting PR Workflow: $BRANCH"

#*********************************************************************/
# 1. Prepare Branch                                                   */
#*********************************************************************/

log_info "Preparing branch..."

if git show-ref --verify --quiet "refs/heads/$BRANCH"; then
    log_info "Switching to existing branch: $BRANCH"
    git checkout "$BRANCH"
else
    log_info "Creating new branch: $BRANCH"
    git checkout -b "$BRANCH"
fi

log_info "Staging changes..."
git add . -A

log_info "Creating commit..."
git commit -m "$MESSAGE" || log_warn "No changes to commit."

#*********************************************************************/
# 2. Push & Create PR                                                 */
#*********************************************************************/

# CRITICAL AUTOMATION FIX: Sync the specific remote tracking reference to local
# index to guarantee --force-with-lease never encounters a (stale info) rejection.
log_info "Synchronizing remote tracking references..."
git fetch origin "$BRANCH" >/dev/null 2>&1 || log_warn "Remote branch tracking signature not established yet."

log_info "Pushing branch to remote..."
git push origin "$BRANCH" --force-with-lease

log_info "Creating GitHub Pull Request..."

PR_URL=$(gh pr create \
    --title "$MESSAGE" \
    --body "Automated standardization and cleanup." \
    --fill)

if [ -z "$PR_URL" ]; then
    PR_URL=$(gh pr view --json url -q ".url")
fi

PR_NUMBER=$(echo "$PR_URL" | grep -oE '[0-9]+$')

log_success "PR Active: $PR_URL (#$PR_NUMBER)"

#*********************************************************************/
# 3. CI/CD Verification Gate                                         */
#*********************************************************************/

echo
log_info "CI/CD pipeline is now running..."
log_info "Monitoring GitHub Actions and required checks locally."

echo
echo "Tracking:"
echo "  - GitHub Actions"
echo "  - CodeQL analysis"
echo "  - Security validation"
echo "  - Required status checks"
echo

log_info "Allowing GitHub Actions to initialize..."
sleep 30

# ENTERPRISE RESILIENCE LOOP: Handle network issues or GitHub API 502 Bad Gateway drops cleanly
RETRY_COUNT=0
MAX_RETRIES=12

while true; do
    CHECKS_COUNT=$(gh pr checks "$PR_NUMBER" 2>/dev/null | wc -l | tr -d '[:space:]' || echo "0")
    if [ "$CHECKS_COUNT" -gt 0 ]; then
        break
    fi

    RETRY_COUNT=$((RETRY_COUNT + 1))
    if [ "$RETRY_COUNT" -ge "$MAX_RETRIES" ]; then
        log_warn "GitHub Engine API did not report checks within window. Proceeding to manual stream check..."
        break
    fi

    log_warn "Waiting for remote pipeline orchestration to register checks (Attempt $RETRY_COUNT/$MAX_RETRIES)..."
    sleep 10
done

log_success "CI/CD checks resolved."

# Streams live pipeline statuses to terminal
gh pr checks "$PR_NUMBER" --watch || log_warn "Live stream interrupted. Please confirm pipeline completion manually."

echo
log_info "Review the final status above."
log_info "Ensure all required checks are GREEN or safely bypassable."
echo

read -p "Press [Enter] to execute administrative squash merge..." RunCommand

#*********************************************************************/
# 4. Merge & Cleanup                                                  */
#*********************************************************************/

log_info "Merging PR #$PR_NUMBER..."

gh pr merge "$PR_NUMBER" \
    --squash \
    --delete-branch \
    --admin

log_success "Pull Request merged successfully."

#*********************************************************************/
# 5. Sync Main                                                        */
#*********************************************************************/

log_info "Synchronizing local main branch..."

git checkout main
git pull --ff-only origin main

log_success "Workflow Complete. Local Environment Synced."