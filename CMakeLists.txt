cmake_minimum_required(VERSION 3.10)

project(pct C)
set(CMAKE_C_STANDARD 99)

set(SRC_FILES
    src/main.c
    src/pct/serial-conv.c
    src/pct/parallel-conv.c
    src/pct/pthreads-conv.c
    src/pct/filters.c
    vendor/argparse.c
)

include_directories(include vendor)

find_package(OpenMP COMPONENTS C REQUIRED)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra)
    message(STATUS "Building in DEBUG mode")
else()
    add_compile_options(-O2)
    message(STATUS "Building in RELEASE mode")
endif()

add_executable(pct ${SRC_FILES})
target_link_libraries(pct PRIVATE m OpenMP::OpenMP_C)
