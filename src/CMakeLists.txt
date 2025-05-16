set(SRC_FILES
    main.c
    pct/serial-conv.c
    pct/parallel-conv.c
    pct/filters.c
    pct/utils.c
    ../vendor/argparse.c
)

add_executable(pct ${SRC_FILES})
add_library(pct-lib STATIC ${SRC_FILES})

target_link_libraries(pct PRIVATE m)
target_link_libraries(pct-lib PRIVATE m)
