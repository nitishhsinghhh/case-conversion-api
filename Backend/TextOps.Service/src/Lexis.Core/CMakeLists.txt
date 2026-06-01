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

# Explicitly locate the absolute path to the nuspell binary
find_library(NUSPELL_LIB NAMES nuspell PATHS "/opt/homebrew/lib")

# ---------------------------------------------------------
# 3. LIBRARY: LexisCore (Business Logic Only)
# ---------------------------------------------------------
add_library(LexisCore SHARED 
    src/SpellChecker.cpp
    src/LexisSpellCheckDLL.cpp
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
    ${NUSPELL_LIB}  # Use the absolute path discovered above
    ICU::uc 
    ICU::data
)

# Ensure data directory exists for the library to use
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/vocabulary")

# ---------------------------------------------------------
# 4. MAIN APPLICATION: LexisApp (The CLI Tool)
# ---------------------------------------------------------
add_executable(LexisApp src/Export.cpp)

# Direct dyld to look for dependent dylibs inside the same directory as the binary
set_target_properties(LexisApp PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "@loader_path"
)

target_link_libraries(LexisApp PRIVATE 
    LexisCore
    ICU::uc
    ICU::data
)

# ---------------------------------------------------------
# 5. GOOGLETEST INTEGRATION
# ---------------------------------------------------------

include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/main.zip
)
FetchContent_MakeAvailable(googletest)

# ---------------------------------------------------------
# 6. TEST EXECUTABLE
# ---------------------------------------------------------
add_executable(runLexisTests
    "/Users/nitishsingh/projects/word-case-api/Backend/TextOps.Service/tests/Lexis.Core.Tests/SpellCheckerTests.cpp"
)

target_include_directories(runLexisTests PRIVATE 
    "${PROJECT_SOURCE_DIR}/../../include"
)

# Explicitly link ICU to the test executable as well
target_link_libraries(runLexisTests PRIVATE 
    LexisCore 
    gtest 
    gtest_main 
    ICU::uc 
    ICU::data
)

add_test(NAME AllTests COMMAND runLexisTests)

# ---------------------------------------------------------
# 7. DLL ABI TEST EXECUTABLE
# ---------------------------------------------------------

add_executable(runLexisDLLTests
    "/Users/nitishsingh/projects/word-case-api/Backend/TextOps.Service/tests/Lexis.Core.Tests/SpellCheckerTestDll.cpp"
)

target_include_directories(runLexisDLLTests PRIVATE
    "${PROJECT_SOURCE_DIR}/../../include"
)

target_link_libraries(runLexisDLLTests PRIVATE
    LexisCore
    gtest
    gtest_main
    ICU::uc
    ICU::data
)

add_test(
    NAME LexisDLLTests
    COMMAND runLexisDLLTests
)