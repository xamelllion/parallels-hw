include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.1
)
FetchContent_MakeAvailable(unity)

set(SRC_FILES
    serial-conv.c
    utils.c
)

add_executable(test-runner ${SRC_FILES})
target_link_libraries(test-runner unity pct-lib)
