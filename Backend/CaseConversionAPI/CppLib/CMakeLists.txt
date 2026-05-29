# -----------------------------------------------------------------------------
# CMake Configuration - Hardware-Aware Case Conversion Engine
# Project      : StringConversion
# Component    : Cross-Platform Native Core + GoogleTest Harness
# Architecture : Static Core + Shared DLL Bridge + CLI + Test Runtime
# -----------------------------------------------------------------------------
# VERSION HISTORY
# Version | Date       | Author        | Description
# --------|------------|---------------|----------------------------------------
# 1.0.0   | 2026-04-14 | Nitish Singh  | Initial native orchestration baseline.
# 1.1.0   | 2026-05-09 | Nitish Singh  | Added Apple Silicon optimization flags
#                                      | and AddressSanitizer integration.
# 1.2.0   | 2026-05-20 | Nitish Singh  | Added Windows MinGW static runtime
#                                      | linking for containerized execution.
# 1.3.0   | 2026-05-28 | Nitish Singh  | Added clang-format automation target
#                                      | and cross-platform formatting support.
# -----------------------------------------------------------------------------
# BUILD STRATEGY
# * Static Core Library        : Shared reusable conversion engine
# * Shared DLL Bridge          : .NET interoperability export layer
# * CLI Runtime                : Standalone native execution target
# * GoogleTest Harness         : Integrated validation framework
# * MinGW Static Linking       : Portable Windows runtime generation
# * Clang-Format Integration   : Automated source formatting pipeline
# * AddressSanitizer Support   : Native debug-time memory diagnostics
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

# 1. Initialize the project profile first so system variables exist
project(StringConversion)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

# 2. Apply explicit static linking constraints globally for cross-compilation
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    # Force underlying components to compile with matching runtime definitions
    set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
else()
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# 3. Restrict AddressSanitizer strictly to native explicit Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Enabling AddressSanitizer for Debug build")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
    endif()
endif()

# ---------------------------
# Global Settings
# ---------------------------
include_directories(include)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---------------------------
# 1. Core Logic: StringConversionLib (STATIC)
# ---------------------------
add_library(StringConversionLib STATIC
    src/AlternatingCaseConversion.cpp
    src/CapitalizeWordsConversion.cpp
    src/Client.cpp
    src/ConversionResult.cpp
    src/InvertWordsConversion.cpp
    src/KebabCaseConversion.cpp
    src/LeetSpeakConversion.cpp
    src/LowerCaseConversion.cpp
    src/ProcessString.cpp
    src/RemoveSpacesConversion.cpp
    src/RemoveVowelsConversion.cpp
    src/ReverseConversion.cpp
    src/SentenceCaseConversion.cpp
    src/SnakeCaseConversion.cpp
    src/StringConversionFactory.cpp
    src/ToggleCaseConversion.cpp
    src/UpperCaseConversion.cpp    
)
target_include_directories(StringConversionLib PUBLIC include)

# ---------------------------
# 2. The Bridge DLL: ProcessStringDLL (SHARED)
# ---------------------------
add_library(ProcessStringDLL SHARED src/ProcessStringDLL.cpp)
target_compile_definitions(ProcessStringDLL PRIVATE PROCESSSTRING_EXPORTS)

if(WIN32)
    target_compile_definitions(ProcessStringDLL PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(ProcessStringDLL PRIVATE StringConversionLib)
set_target_properties(ProcessStringDLL PROPERTIES PREFIX "lib")

# ---------------------------
# 3. Main Application (CLI)
# ---------------------------
add_executable(app src/sourcecode.cpp)
target_link_libraries(app StringConversionLib)

# ---------------------------
# 4. GoogleTest Setup
# ---------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/main.zip
)
FetchContent_MakeAvailable(googletest)

# ---------------------------
# 5. Test Executable
# ---------------------------
add_executable(runTests
    ${PROJECT_SOURCE_DIR}/../Tests/CppTests/StringConversionTests.cpp
    ${PROJECT_SOURCE_DIR}/../Tests/CppTests/AdvancedStringConversionTests.cpp
)
target_link_libraries(runTests StringConversionLib gtest gtest_main)

# Force the test runner to link dependencies statically under MinGW
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_options(runTests PRIVATE "-static" "-static-libgcc" "-static-libstdc++")
endif()

add_test(NAME AllTests COMMAND runTests)

# ---------------------------
# 6. Code Formatting (Clang-Format Automation)
# ---------------------------
find_program(CLANG_FORMAT_EXE 
    NAMES clang-format
    HINTS /opt/homebrew/bin /usr/local/bin
)

if(CLANG_FORMAT_EXE)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_EXE}")
    file(GLOB_RECURSE ALL_FORMAT_FILES 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/../Tests/CppTests/*.cpp"
    )
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=LLVM ${ALL_FORMAT_FILES}
        COMMENT "Auto-formatting all C++ engine source..."
    )
endif()