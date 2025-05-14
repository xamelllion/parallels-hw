cmake_minimum_required(VERSION 3.10)

project(main C)
set(CMAKE_C_STANDARD 99)

set(SRC_FILES
    src/main.c
    src/serial-conv.c
    src/parallel-conv.c
    src/filters.c
    vendor/argparse.c
)

include_directories(include vendor)

find_package(OpenMP COMPONENTS C REQUIRED)

add_executable(main ${SRC_FILES})
target_link_libraries(main PRIVATE m OpenMP::OpenMP_C)
