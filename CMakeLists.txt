cmake_minimum_required(VERSION 3.24)
project(team08 C)

set(CMAKE_C_STANDARD 23)
add_compile_options(-Wall -Wextra -Wpedantic -O3 -ffast-math -march=native)
#add_compile_options(-Wall -Wextra -Wpedantic -O0)

include_directories(. /usr/local/include /usr/include)
add_subdirectory(src)
add_subdirectory(tests)




