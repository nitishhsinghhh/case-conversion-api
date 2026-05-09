#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
#  Utility Script - Telemetry Infrastructure (Jaeger/OTLP)           */
#  Version     : 1.2                                                 */
#                                                                    */
# Purpose   : Manages the lifecycle of the Jaeger container for      */
#             OpenTelemetry trace collection and visualization.      */
# Usage     : backend/CaseConversionAPI/Scripts/provision-telemetry.sh 
#             [start|stop|status]                                    */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date           Author           Description             */
# ------------------------------------------------------------------ */
# 1.0        2026-04-18     Nitish Singh     Initial implementation. */
# 1.1        2026-05-09     Nitish Singh     Added explicit version  */
#                                            tag to header.          */
# 1.2        2026-05-09     Nitish Singh     Standardized log,       */
#                                            added log follow cmd.   */
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
# Configuration                                                      */
#*********************************************************************/
CONTAINER_NAME="jaeger"
IMAGE="jaegertracing/all-in-one:latest"

#*********************************************************************/
# Core Logic                                                         */
#*********************************************************************/
case "${1:-start}" in
    start)
        log_info "===== Starting Telemetry Infrastructure ====="
        
        if [ "$(docker ps -aq -f name=$CONTAINER_NAME)" ]; then
            if [ "$(docker ps -q -f name=$CONTAINER_NAME)" ]; then
                log_success "Jaeger is already running."
            else
                log_info "Restarting existing Jaeger container..."
                docker start $CONTAINER_NAME
            fi
        else
            log_info "Provisioning new Jaeger container..."
            docker run -d --name $CONTAINER_NAME \
              -e COLLECTOR_OTLP_ENABLED=true \
              -p 16686:16686 \
              -p 4317:4317 \
              -p 4318:4318 \
              $IMAGE
        fi
        log_success "Jaeger UI available at: http://localhost:16686"
        ;;

    stop)
        log_info "===== Stopping Telemetry Infrastructure ====="
        if [ "$(docker ps -q -f name=$CONTAINER_NAME)" ]; then
            docker stop $CONTAINER_NAME
            log_success "Telemetry stopped."
        else
            log_warn "No active Jaeger container found to stop."
        fi
        ;;

    status)
        log_info "===== Telemetry Status ====="
        docker ps -f name=$CONTAINER_NAME
        ;;

    logs)
        log_info "===== Tailing Telemetry Logs ====="
        docker logs -f $CONTAINER_NAME
        ;;

    *)
        log_error "Invalid argument: $1"
        echo -e "${YELLOW}Usage:${NC} $0 {start|stop|status|logs}"
        exit 1
        ;;
esac