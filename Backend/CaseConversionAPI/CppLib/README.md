# Native Conversion Engine (C++17)

The "Source of Truth" for the system. A stateless, high-performance library designed for zero-overhead string manipulation.

## Architectural Principles

- Decoupled Logic: Utilizes the Strategy Pattern to isolate conversion algorithms and the Factory Pattern for runtime selection.

- Binary Compatibility: Uses a C-style ABI (extern "C") to ensure the shared library is consumable by any runtime (.NET, Python, Go) without name mangling issues.

- Memory Governance: Implements an explicit Ownership Transfer model. Since C++ allocates on the unmanaged heap, it exports a freeString function to ensure the calling environment can signal resource reclamation, achieving a "Zero-Leak" lifecycle.

- Performance & Complexity: The engine achieves O(n) time complexity and O(n) space complexity for all transformations. String processing is performed in a single pass where possible to minimize cache misses and optimize CPU cycles.

- Security & Input Sanitization: The API utilizes strict Buffer Bounds Checking. Input strings exceeding a specific threshold (e.g., 2MB) are rejected at the boundary to prevent heap exhaustion or Buffer Overflow attempts.

## Operational Setup

```Bash
# Production Build Sequence
    mkdir -p build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build . --config Release --parallel $(nproc)
```

```Bash
cmake ..
make
./runTests
```

## Quality Assurance

Comprehensive validation via GoogleTest covering edge cases (empty strings, special characters, and maximum buffer sizes).

```Bash
./runTests --gtest_repeat=10 --gtest_break_on_failure
```

## Project Structure

```Bash

CaseConversionAPI/
├── CppLib/
│   ├── include/          # Public headers
│   ├── src/              # Implementation
│   ├── CMakeLists.txt    # Library build
│   └── run.sh            # Local build script
├── tests/
│   └── CppTests/         # Unit tests (GoogleTest)
└── README.md


```

## Architecture

The project follows a modular design using:

- Strategy Pattern for conversion implementations
- Factory Pattern for runtime object creation
- Dispatcher layer for conversion selection
- CLI interface for testing
- DLL wrapper for interoperability

Flow:

```Bash
CLI / API → ProcessString → Factory → Strategy → Result`
```

## Using as Library

```cpp
#include "ProcessString.hpp"

std::string result = processString("hello world", 11);
```

---

## Conversion Table

```markdown
## Supported Conversions

| ID | Conversion |
|----|------------|
| 1  | Alternating |
| 2  | Capitalize |
| 3  | Lower |
| 4  | Upper |
| 5  | Sentence |
| 6  | Toggle |
| 7  | Reverse |
| 8  | RemoveVowels |
| 9  | RemoveSpaces |
| 10 | InvertWords |
| 11 | SnakeCase |
| 12 | KebabCase |
| 13 | LeetSpeak |

## Design Goals

- Modular conversion strategies
- Easy extensibility
- Modern C++ memory safety
- Test-driven validation
- Cross-platform build support

## Future Enhancements

- Unicode support
- Plugin-based conversions
- REST API integration
- Performance benchmarks

---


## Running the Application

A convenient script `run.sh` is provided to build the project and run both the main application and tests.

### Steps

1.Make the script executable** (only need to do this once):

```bash
cd CaseConversionAPI/CppLib
chmod +x run.sh
```

2.Run the application and tests

```Bash
./run.sh
```

This script will:

- Create the build directory if it doesn’t exist
- Configure and build the project using CMake
- Run the main application (app)
- Run all unit tests (runTests)

## Running manually

Alternatively, you can run manually:

```Bash
mkdir -p build
cd build
cmake ..
cmake --build . --config Release

# Run the application
./app

# Run unit tests
./runTests
```

## Main Application Usage

When running the app:

```Bash
Enter string: hello world
1: Alternating, 2: Capitalize, 3: Lower, 4: Upper, 5: Sentence, 6: Toggle, 7: Reverse, 8: RemoveVowels, 9: RemoveSpaces, 10: InvertWords, 11: SnakeCase, 12: KebabCase, 13: LeetSpeak
```

- Enter the string you want to convert
- Enter the choice number corresponding to the conversion strategy
- Result will be printed on the console

Running Tests

Unit tests are built with GoogleTest. Running ./runTests will execute all test suites and display results:

```Bash
[==========] Running 34 tests from 19 test suites.
[  PASSED  ] 34 tests.
```

---

## Git Setup and .gitignore

To properly manage this project with Git:

1. **Initialize Git** (if not already initialized):

```bash
git init
```

2.Add the remote repository (replace with your repo URL):

```Bash
git remote add origin https://github.com/nitishhsinghhh/case-conversion-api.git
```

3.Create a .gitignore file in the root of the project:

```Bash
touch .gitignore
```

4.Add common C++ / CMake ignore rules. Example contents:

```Bash
# Build directories
/build/
/bin/
/lib/

# CMake files
CMakeFiles/
CMakeCache.txt
cmake_install.cmake
Makefile

# OS-specific files
.DS_Store
Thumbs.db

# IDE files
*.vcxproj*
*.sln
*.user
*.idea/
*.vscode/
```

5.Stage and commit changes:

```Bash
git add .
git commit -m "Add project files and .gitignore"
```

6.Push to remote repository:

```Bash
git push -u origin main
```

After this, Git will ignore all the files and folders specified in .gitignore, keeping your repository clean and only tracking source and configuration files.

---

## C++ Logic Component

This directory contains the core C++ logic for the Case Conversion API. 

## Build System "Swap" Trick

To maintain a clean separation between the production API and local testing, we use two distinct CMake configurations:

1. `CMakeLists.txt` (Production): The default configuration. It builds the project as a shared library or module intended to be consumed by the backend API.
2. `CMakeListsLocalApp.txt` (Testing/Local): A specialized configuration that builds a standalone executable. This version includes a `main()` entry point for rapid testing and logic verification.

### Why we do

In our CI/CD pipeline (GitHub Actions), we perform a "Hot-Swap" of these files:

```bash
cp CMakeListsLocalApp.txt CMakeLists.
```

This allows the pipeline to compile and run the logic as a standalone application to verify string conversion accuracy without needing to boot up the entire web server infrastructure.

## How to Build Locally

1. Swap the files:

```Bash
cp CMakeListsLocalApp.txt CMakeLists.txt
```

2. Configure and Build:

```Bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

3. Run the App:

```Bash
./build/CaseConversionApp
```

Note: Be careful not to commit the swapped CMakeLists.txt if you have made changes to the production version. Always revert or use git checkout CMakeLists.txt after local testing.

---
