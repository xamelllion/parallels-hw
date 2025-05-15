cmake_minimum_required(VERSION 3.10)

project(pct C)
set(CMAKE_C_STANDARD 99)

set(SRC_FILES
    src/main.c
    src/pct/serial-conv.c
    src/pct/parallel-conv.c
    src/pct/filters.c
    src/pct/utils.c
    vendor/argparse.c
)

include_directories(include vendor)

set(CMAKE_BUILD_TYPE "Release")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra)
    message(STATUS "Building in DEBUG mode")
else()
    add_compile_options(-O2)
    message(STATUS "Building in RELEASE mode")
endif()

add_executable(pct ${SRC_FILES})
target_link_libraries(pct PRIVATE m)
