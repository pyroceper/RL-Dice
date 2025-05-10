cmake_minimum_required(VERSION 3.15)
project(rldice)

# set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# output directories for build artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# define the RL-Dice library
add_library(rldice STATIC
    src/dice.cpp
)

# include directories for RL-Dice library
target_include_directories(rldice PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# define the test executable
add_executable(test tests/test.cpp)

# link RL-Dice library to the test executable
target_link_libraries(test PRIVATE rldice)

