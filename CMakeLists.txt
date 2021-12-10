cmake_minimum_required(VERSION 3.21)
project(HTML_Generator C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(HTML_Generator
        main.c
        contents.c
        helpers.c)
