cmake_minimum_required(VERSION 3.2)
project(unittests)

set(CMAKE_CXX_STANDARD 14)

include_directories(. /usr/local/include /usr/include
        ../src)
find_library(JPEGLIB REQUIRED NAMES libjpeg.a PATHS "usr/local/lib")
set(SOURCE_FILES main.cpp test_min_cut.cpp test_image.cpp ../src/quilting.c ../src/calc_errors.c
        ../src/load_image.c ../src/image.c ../src/matrix.c ../src/min_cut.c)

add_executable(unittests ${SOURCE_FILES})
target_link_libraries(unittests gtest ${JPEGLIB})

