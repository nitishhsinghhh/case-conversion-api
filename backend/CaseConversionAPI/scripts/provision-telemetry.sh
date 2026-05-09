#!/bin/bash
#*********************************************************************/
#  Utility Script - Telemetry Infrastructure (Jaeger/OTLP)            */
#  Version     : 1.1                                                 */
#                                                                    */
# Purpose   : Manages the lifecycle of the Jaeger container for      */
#             OpenTelemetry trace collection and visualization.      */
# Usage     : ./run-telemetry.sh [start|stop|status]                 */
#                                                                    */
# Revision History:                                                  */
# ------------------------------------------------------------------ */
# Version    Date           Author           Description              */
# ------------------------------------------------------------------ */
# 1.0        2026-04-18     Nitish Singh     Initial implementation. */
# 1.1        2026-05-09     Nitish Singh     Added explicit version  */
#                                            tag to header.          */
#*********************************************************************/

CONTAINER_NAME="jaeger"

case "$1" in
    start|"")
        echo "===== Starting Telemetry Infrastructure ====="
        if [ "$(docker ps -aq -f name=$CONTAINER_NAME)" ]; then
            if [ "$(docker ps -q -f name=$CONTAINER_NAME)" ]; then
                echo "Jaeger is already running."
            else
                echo "Starting existing Jaeger container..."
                docker start $CONTAINER_NAME
            fi
        else
            echo "Creating and starting new Jaeger container..."
            docker run -d --name $CONTAINER_NAME \
              -e COLLECTOR_OTLP_ENABLED=true \
              -p 16686:16686 \
              -p 4317:4317 \
              -p 4318:4318 \
              jaegertracing/all-in-one:latest
        fi
        echo "Jaeger UI available at: http://localhost:16686"
        ;;
    stop)
        echo "===== Stopping Telemetry Infrastructure ====="
        docker stop $CONTAINER_NAME || echo "Container not running."
        ;;
    status)
        echo "===== Telemetry Status ====="
        docker ps -f name=$CONTAINER_NAME
        ;;
    *)
        echo "Usage: $0 {start|stop|status}"
        exit 1
esac