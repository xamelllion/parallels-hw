set(SRC_FILES
    pct/serial-conv.c
    pct/parallel-conv.c
    pct/filters.c
    pct/utils.c
    ../vendor/argparse.c
)

add_library(pct-lib STATIC ${SRC_FILES})
target_link_libraries(pct-lib PRIVATE m)

add_executable(pct main.c)
target_link_libraries(pct PRIVATE pct-lib)
