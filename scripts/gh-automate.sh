#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
# File      : gh-automate.sh                                         */
# Version   : 1.2                                                    */
#                                                                    */
# Purpose   : Local PR Automator for standardization and cleanup.    */
#             Handles branch creation, PR submission, and merging.   */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date        Author          Description                 */
# ------------------------------------------------------------------ */
# 1.0        2026-04-14  Nitish Singh    Initial Automation Script.  */
# 1.1        2026-05-09  Nitish Singh    Added architecture sync and */
#                                        M2 cleanup verification.    */
# 1.2        2026-06-01  Nitish Singh    Refined manual checkpoint   */
#                                        and added final sync steps. */
# 1.3        2026-06-10  Nitish Singh    Open PR in browser for CI 
#                                        monitoring                  */
#*********************************************************************/

BRANCH=$1
MESSAGE=$2

if [ -z "$BRANCH" ] || [ -z "$MESSAGE" ]; then
    echo "Error: Missing arguments."
    echo "Usage: ./gh-automate.sh <branch-name> <message>"
    exit 1
fi

echo "===== Starting PR Workflow: $BRANCH ====="

# 1. Prepare Branch
if git show-ref --verify --quiet "refs/heads/$BRANCH"; then
    git checkout "$BRANCH"
else
    git checkout -b "$BRANCH"
fi

git add . -A 
git commit -m "$MESSAGE" || echo "No changes to commit"

# 2. Push & Create PR
git push origin "$BRANCH" --force-with-lease
PR_URL=$(gh pr create --title "$MESSAGE" --body "Automated standardization and cleanup." --fill)

if [ -z "$PR_URL" ]; then
    PR_URL=$(gh pr view --json url -q ".url")
fi

PR_NUMBER=$(echo "$PR_URL" | grep -oE '[0-9]+$')
echo "PR Active: $PR_URL (#$PR_NUMBER)"

# Open PR in browser for CI/CD monitoring
echo "Opening PR in browser..."
gh pr view "$PR_NUMBER" --web

# 3. The "Blue Tick" Manual Gate
echo "--------------------------------------------------------"
echo "Waiting for CI/CD (CodeQL, etc.) to start/complete..."
echo "Please monitor the PR in your browser or run 'gh pr checks'."
echo "Once you see all GREEN checkmarks (or bypassable the ticks):"
read -p "Press [Enter] to proceed with the merge..."
echo "--------------------------------------------------------"

# 4. Merge & Cleanup
echo "Merging PR #$PR_NUMBER..."
gh pr merge "$PR_NUMBER" --squash --delete-branch --admin

# 5. Sync Main
git checkout main
git pull origin main

echo -e "\n===== Workflow Complete. Environment Synced. ====="