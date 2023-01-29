cmake_minimum_required(VERSION 3.22.1)

project(sjson C)

# ============== Lib ===================

add_library(sjson STATIC
    src/lexer.c
    src/utils/stringReader.c
)

target_include_directories(sjson PUBLIC ${CMAKE_SOURCE_DIR}/include/public)
target_include_directories(sjson PRIVATE ${CMAKE_SOURCE_DIR}/include/private)

set_target_properties(sjson PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist
)

# ============== Main ==================

add_executable(main
    main.c
)

target_link_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dist/)
target_link_libraries(main sjson)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist-main/
)