cmake_minimum_required(VERSION 3.22.1)

project(sjson C)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(sjson STATIC
    src/test.c
)

set_target_properties(sjson PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist
)

# ===================

add_executable(main
    main.c
)

target_link_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dist/)
target_link_libraries(main sjson)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)