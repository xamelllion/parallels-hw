include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.1
)
FetchContent_MakeAvailable(unity)

add_executable(serial-conv-test serial-conv.c utils.c)
target_link_libraries(serial-conv-test unity pct-lib)

add_executable(parallel-conv-test parallel-conv.c utils.c)
target_link_libraries(parallel-conv-test unity pct-lib)
