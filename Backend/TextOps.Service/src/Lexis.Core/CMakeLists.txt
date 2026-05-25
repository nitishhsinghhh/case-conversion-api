cmake_minimum_required(VERSION 3.14)
project(LexisEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional flag: Run `cmake -DBUILD_CLI=ON ...` to compile the local runner instead
option(BUILD_CLI "Build the standalone CLI binary wrapper" OFF)

# =========================================================
# 1. SETUP EXTERNAL PATHS (ICU/Homebrew)
# =========================================================
if(APPLE)
    set(ICU_ROOT "/opt/homebrew/opt/icu4c")
    list(APPEND CMAKE_PREFIX_PATH "${ICU_ROOT}" "/opt/homebrew")
endif()

# =========================================================
# 2. FIND DEPENDENCIES
# =========================================================
find_package(ICU 78.3 REQUIRED COMPONENTS uc data)
find_package(nuspell REQUIRED)

if(APPLE)
    find_library(NUSPELL_LIB NAMES nuspell PATHS "/opt/homebrew/lib")
else()
    find_library(NUSPELL_LIB NAMES nuspell)
endif()

# =========================================================
# 3. STATIC CORE LOGIC LIBRARY
# =========================================================
add_library(LexisCore STATIC 
    src/SpellChecker.cpp
)

target_compile_definitions(LexisCore PUBLIC 
    DICTIONARY_PATH="${CMAKE_CURRENT_SOURCE_DIR}/vocabulary/dictionary.txt"
)

target_include_directories(LexisCore PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(APPLE)
    target_include_directories(LexisCore PUBLIC "/opt/homebrew/include" "${ICU_ROOT}/include")
endif()

target_link_libraries(LexisCore PRIVATE 
    ${NUSPELL_LIB}
    ICU::uc 
    ICU::data
)

# Ensure baseline vocabulary workspace structure exists
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/vocabulary")

# =========================================================
# 4. CONDITIONAL TARGET COMPILATION SWITCH
# =========================================================
if(BUILD_CLI)
    # Target A: Standalone CLI Application Context
    add_executable(LexisApp src/Export.cpp)

    set_target_properties(LexisApp PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@loader_path"
    )

    target_link_libraries(LexisApp PRIVATE 
        LexisCore
        ICU::uc
        ICU::data
    )
    message(STATUS "Configured target compilation for: Local CLI Application (LexisApp)")

else()
    # Target B: Production Unmanaged Interop Dynamic Library (Default)
    add_library(LexisSpellCheckDLL SHARED 
        src/LexisSpellCheckDLL.cpp
    )

    # Handles safe Windows dllexport scoping while keeping macOS symbol tables broad
    target_compile_definitions(LexisSpellCheckDLL PRIVATE LEXISSPELLCHECK_EXPORTS)
    
    # Track includes across both the standard public API headers and localized source files
    target_include_directories(LexisSpellCheckDLL PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )

    target_link_libraries(LexisSpellCheckDLL PRIVATE 
        LexisCore
        ICU::uc
        ICU::data
    )

    if(APPLE)
        set_target_properties(LexisSpellCheckDLL PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@loader_path"
            MACOSX_RPATH TRUE
        )
    endif()
    message(STATUS "Configured target compilation for: Shared Interop Layer (libLexisSpellCheckDLL)")
endif()