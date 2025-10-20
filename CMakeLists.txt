cmake_minimum_required(VERSION 3.16)
project(RISCVPerfSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Wextra -Wpedantic -O2 -g)

include_directories(${CMAKE_SOURCE_DIR}/src)

# gather source files
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(riscv_perf_sim ${SOURCES})

# enable testing
enable_testing()
add_subdirectory(tests)