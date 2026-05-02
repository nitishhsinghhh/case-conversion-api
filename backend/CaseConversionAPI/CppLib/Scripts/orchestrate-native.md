# C++ Core Orchestration Guide (Monorepo)

This guide provides the end-to-end instructions for building, optimizing, and testing the high-performance C++ engine across different environments using the orchestrate-native.sh script.

## 1. macOS (Local / Native)

Targeting Apple M2 Silicon for maximum performance.

### Setup

1. Install Tooling: Ensure you have CMake and the Apple Command Line Tools.

```Bash
brew install cmake
xcode-select --install
```

2. Permissions: Make the script executable.

```Bash
chmod +x backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh
```

Run

Run directly from the project root:

```Bash
./backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh macos-latest
```

### The Fix: Purge the Cache

You must delete the existing build artifacts before switching environments. Run this in your Mac terminal:

```Bash
# 1. Clear the corrupted cache
rm -rf backend/CaseConversionAPI/CppLib/build/*

# 2. Run the script again
./backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh macos-latest
```

## 2. Linux (Docker)

Targeting a clean Ubuntu environment for cloud-native validation.

Setup (Initial Login)

Build the Environment:

```Bash
docker build -t cpp-linux-env -f - <<EOF .
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y build-essential cmake git
WORKDIR /app
EOF
```

### Mount & Enter (Login)

```Bash
docker rm -f cpp-dev
docker run -it --rm --name cpp-dev -v "$(pwd)":/app cpp-linux-env /bin/bash
```

### Run

Inside the Docker shell:

```Bash
./backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh ubuntu-latest
```

## 3. Windows (Docker Cross-Build)

Targeting Windows verification from your Mac using MinGW or MSVC.

Setup

To compile Windows binaries without leaving your Mac, use a specialized Docker container:

Create Windows-Toolchain Image:

Clear the Windows artifacts:

```Bash
rm -rf backend/CaseConversionAPI/CppLib/build/*
```

```Bash
docker build -t cpp-win-env -f - . <<EOF
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y build-essential cmake mingw-w64 make
WORKDIR /app
EOF
```

Run

Run as a "one-shot" command from the project root:

```Bash
docker run --rm -v "$(pwd)":/app cpp-win-env \
/bin/bash ./backend/CaseConversionAPI/CppLib/Scripts/orchestrate-native.sh windows-latest
```
