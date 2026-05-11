cmake_minimum_required(VERSION 3.14)
project(LexisEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

# 1. SETUP EXTERNAL PATHS (ICU/Homebrew)
set(ICU_ROOT "/opt/homebrew/opt/icu4c")
list(APPEND CMAKE_PREFIX_PATH "${ICU_ROOT}" "/opt/homebrew")

# 2. FIND DEPENDENCIES
find_package(ICU 78.3 REQUIRED COMPONENTS uc data)
find_package(nuspell REQUIRED)
find_package(GTest REQUIRED)
include(GoogleTest)

# ---------------------------------------------------------
# 3. LIBRARY: LexisCore (Business Logic Only)
# ---------------------------------------------------------
# We EXCLUDE Export.cpp here so the library has no main()
add_library(LexisCore SHARED 
    src/SpellChecker.cpp
)

target_compile_definitions(LexisCore PUBLIC 
    DICTIONARY_PATH="${CMAKE_CURRENT_SOURCE_DIR}/vocabulary/dictionary.txt"
)

target_include_directories(LexisCore PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "/opt/homebrew/include"
    "${ICU_ROOT}/include"
)

target_link_libraries(LexisCore PRIVATE 
    nuspell
    ICU::uc 
    ICU::data
)

# Ensure data directory exists for the library to use
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/vocabulary")

# ---------------------------------------------------------
# 4. MAIN APPLICATION: LexisApp (The CLI Tool)
# ---------------------------------------------------------
# This contains Export.cpp which has your main() loop
add_executable(LexisApp src/Export.cpp)

target_link_libraries(LexisApp PRIVATE 
    LexisCore
    ICU::uc
    ICU::data
)

# ---------------------------------------------------------
# 5. TEST EXECUTABLE: LexisCoreTests (GoogleTest)
# ---------------------------------------------------------
add_executable(LexisCoreTests 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../tests/Lexis.Core.Tests/SpellCheckerTests.cpp
)

# Link against the logic and the GTest runner
target_link_libraries(LexisCoreTests PRIVATE 
    LexisCore
    GTest::gtest_main
    ICU::uc
    ICU::data
)

# Register tests with CTest and Discover TEST() macros
add_test(NAME CoreLogicTests COMMAND LexisCoreTests)
gtest_discover_tests(LexisCoreTests)

# Apple-specific linker fix for Homebrew paths
if(APPLE)
    target_link_directories(LexisCore PRIVATE "/opt/homebrew/lib")
    target_link_directories(LexisCoreTests PRIVATE "/opt/homebrew/lib")
endif()