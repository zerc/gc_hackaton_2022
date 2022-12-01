cmake_minimum_required(VERSION 3.23...3.25)

project(
    GoCardless
    VERSION 0.1
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(vendor/raylib)

add_subdirectory(src)

add_custom_target(copy-assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)
add_dependencies(gc_exec copy-assets)

enable_testing()