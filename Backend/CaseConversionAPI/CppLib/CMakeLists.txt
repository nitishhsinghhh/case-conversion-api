# -----------------------------------------------------------------------------
# CMake Configuration - StringConversion Native Engine
# Project      : StringConversion
# Component    : Core Library + CLI + GoogleTest Validation
# -----------------------------------------------------------------------------
# VERSION HISTORY
# Version | Date       | Author        | Description
# --------|------------|---------------|----------------------------------------
# 1.0.0   | 2026-04-14 | Nitish Singh  | Initial native CMake orchestration
#                                      | with GoogleTest integration.
# 1.1.0   | 2026-05-28 | Nitish Singh  | Added automated clang-format target
#                                      | for recursive source/test formatting.
# -----------------------------------------------------------------------------
# BUILD ARCHITECTURE
# * Static Core Library        : Shared string conversion engine
# * CLI Runtime                : Native executable entry point
# * GoogleTest Integration     : Unit + advanced validation suite
# * Clang-Format Automation    : Consistent code-style enforcement
# * Recursive Source Discovery : Automated formatting coverage
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0135 NEW)

project(StringConversion)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

# -----------------------------------------------------------------------------
# Include directories
# -----------------------------------------------------------------------------

include_directories(include)

# -----------------------------------------------------------------------------
# Library: StringConversionLib
# -----------------------------------------------------------------------------

set(BUILD_SHARED_LIBS ON)

add_library(StringConversionLib
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

# -----------------------------------------------------------------------------
# Main Application
# -----------------------------------------------------------------------------

add_executable(app src/sourcecode.cpp)
target_link_libraries(app StringConversionLib)

# -----------------------------------------------------------------------------
# GoogleTest Setup
# -----------------------------------------------------------------------------
include(FetchContent)

# Force GoogleTest to use the same runtime on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/main.zip
)

# Make GoogleTest available
FetchContent_MakeAvailable(googletest)

# -----------------------------------------------------------------------------
# Test Executable
# -----------------------------------------------------------------------------
add_executable(runTests
    ${PROJECT_SOURCE_DIR}/../Tests/CppTests/StringConversionTests.cpp
    ${PROJECT_SOURCE_DIR}/../Tests/CppTests/AdvancedStringConversionTests.cpp
)

# Link the library and GoogleTest
target_link_libraries(runTests StringConversionLib gtest gtest_main)

# -----------------------------------------------------------------------------
# Register Tests with CTest
# -----------------------------------------------------------------------------

add_test(NAME AllTests COMMAND runTests)

# -----------------------------------------------------------------------------
# 6. Code Formatting (Clang-Format Automation)
# -----------------------------------------------------------------------------

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
        COMMENT "Auto-formatting C++ engine, application, and test suites..."
    )
else()
    message(WARNING "clang-format executable not found. 'format' target will not be available.")
endif()