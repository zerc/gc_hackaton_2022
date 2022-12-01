set(SOURCES 
    utils.c
)

add_library(gc_lib ${SOURCES})
target_include_directories(gc_lib PUBLIC "${GoCardless_SOURCE_DIR}/include")

add_executable(gc_exec main.c)
target_link_libraries(gc_exec PRIVATE gc_lib raylib)