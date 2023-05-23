cmake_minimum_required(VERSION 3.2)
project(unittests)

set(CMAKE_CXX_STANDARD 14)

include_directories(. /usr/local/include /usr/include
        ../src)
find_library(JPEGLIB REQUIRED NAMES libjpeg.a PATHS "usr/local/lib")
set(
        SOURCE_FILES main.cpp
        test_min_cut.cpp
        test_image.cpp
        ../src/image.c
        ../src/matrix.c

        ../src/baseline/quilting.c
        ../src/baseline/calc_errors.c
        ../src/load_image.c
        ../src/baseline/min_cut.c

        ../src/opt_6/quilting_opt_6.c
        ../src/opt_6/calc_errors_opt_6.c
        ../src/opt_6/min_cut_opt_6.c
)

add_executable(unittests ${SOURCE_FILES})
target_link_libraries(unittests gtest ${JPEGLIB})

