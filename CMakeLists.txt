cmake_minimum_required(VERSION 3.10)

project(pct C)
set(CMAKE_C_STANDARD 99)

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra)
    message(STATUS "Building in DEBUG mode")
else()
    add_compile_options(-O2)
    message(STATUS "Building in RELEASE mode")
endif()

include_directories(include vendor)

add_subdirectory(src)
add_subdirectory(tests)
