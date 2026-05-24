# -----------------------------------------------------------------------------
# Microsoft Azure Container Registry / Node.js Static Web Template
# Project:      Hardware-Aware Case Conversion Engine
# Component:    Frontend React Production Bundle (Vite + Nginx)
# Documentation: https://github.com/nitishhsinghhh/case-conversion-api
# -----------------------------------------------------------------------------
# VERSION HISTORY
# Version | Date       | Author       | Description
# --------|------------|--------------|----------------------------------------
# 1.0.0   | 2026-04-21 | Nitish Singh | Initial Release. Optimized multi-stage build.
# -----------------------------------------------------------------------------
# ARCHITECTURAL STRATEGY:
# * Multi-Stage Build: Uses Node.js for compilation and Nginx for serving.
# * Alpine Linux: Utilizes minimal footprints to reduce attack surface.
# * Dist-Promotion: Transfers only static assets to the final production image.
# -----------------------------------------------------------------------------

# --- STAGE 1: COMPILATION (BUILD) ---
FROM node:20-alpine AS build
WORKDIR /app

# Dependency Management: Optimized layer caching
COPY package*.json ./
RUN npm install

# Asset Compilation
COPY . .
RUN npm run build

# --- STAGE 2: PRODUCTION RUNTIME ---
FROM nginx:stable-alpine AS runtime

# Artifact Promotion: Copy Vite build output to Nginx root
COPY --from=build /app/dist /usr/share/nginx/html

# System Configuration
EXPOSE 80

# Execute Nginx in the foreground for container persistence
CMD ["nginx", "-g", "daemon off;"]