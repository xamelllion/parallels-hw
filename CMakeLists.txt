cmake_minimum_required(VERSION 3.10)

project(MyExecutable C)
set(CMAKE_C_STANDARD 99)

set(SRC_FILES
    src/1.c
)

add_executable(1 ${SRC_FILES})
