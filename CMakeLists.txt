cmake_minimum_required(VERSION 3.9)
project(apdl_highlighter)

set(CMAKE_CXX_STANDARD 11)

add_executable(apdl_highlighter
        main.cpp
        common.h
        common.cpp
        highlight_config.cpp
        highlight_config.h
        highlighter.cpp
        highlighter.h)

# Copy contents of 'input' to build directory
add_custom_command(TARGET apdl_highlighter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/input"
        $<TARGET_FILE_DIR:apdl_highlighter>/input)
add_custom_command(TARGET apdl_highlighter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/config"
        $<TARGET_FILE_DIR:apdl_highlighter>/config)