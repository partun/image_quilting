cmake_minimum_required(VERSION 3.24)

project(team08 C)

set(CMAKE_C_STANDARD 23)

include_directories(. /usr/local/include)

find_library(JPEGLIB REQUIRED NAMES libjpeg.a PATHS "usr/local/lib")
add_executable(team08
        src/main.c src/load_image.c src/calc_errors.c)
target_link_libraries(team08 PRIVATE ${JPEGLIB})


