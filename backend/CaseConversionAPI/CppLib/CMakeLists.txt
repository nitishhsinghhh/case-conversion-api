cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

# Add this near the top, after project()
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Enabling AddressSanitizer for Debug build")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
    endif()
endif()

project(StringConversion)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

# ---------------------------
# Global Settings
# ---------------------------
include_directories(include)
# Required for linking static code into a shared library on some platforms
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
# This creates the actual file (libProcessStringDLL.dylib / .so) for .NET
add_library(ProcessStringDLL SHARED src/ProcessStringDLL.cpp)

# This tells the compiler "We are BUILDING the DLL, not using it"
target_compile_definitions(ProcessStringDLL PRIVATE PROCESSSTRING_EXPORTS)

# This silences the 'strcpy' warning (C4996) seen in your logs
if(WIN32)
    target_compile_definitions(ProcessStringDLL PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(ProcessStringDLL PRIVATE StringConversionLib)

# Ensure the "lib" prefix is consistent for your scripts
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
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/main.zip
)
FetchContent_MakeAvailable(googletest)

# ---------------------------
# 5. Test Executable
# ---------------------------
add_executable(runTests
    ${PROJECT_SOURCE_DIR}/../tests/CppTests/StringConversionTests.cpp
    ${PROJECT_SOURCE_DIR}/../tests/CppTests/AdvancedStringConversionTests.cpp
)
target_link_libraries(runTests StringConversionLib gtest gtest_main)

add_test(NAME AllTests COMMAND runTests)