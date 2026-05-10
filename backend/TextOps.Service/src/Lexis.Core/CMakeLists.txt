cmake_minimum_required(VERSION 3.14)
# Required for modern FetchContent/ExternalProject behaviors
cmake_policy(SET CMP0135 NEW)

project(ProcessString)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------
# Library: ProcessStringLib
# ---------------------------
add_library(ProcessStringLib src/Export.cpp)

# Use target_include_directories instead of include_directories 
# This ensures anything linking to this lib gets the headers automatically
target_include_directories(ProcessStringLib PUBLIC include)

# ---------------------------
# Main Application
# ---------------------------
# Link the library instead of recompiling Export.cpp
add_executable(app main.cpp) 
target_link_libraries(app PRIVATE ProcessStringLib)
