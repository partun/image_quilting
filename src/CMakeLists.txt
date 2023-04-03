
find_library(JPEGLIB REQUIRED NAMES libjpeg.a PATHS "usr/local/lib")
add_executable(team08
        main.c load_image.c calc_errors.c quilting.c quilting.c min_cut.c min_cut.h matrix.c matrix.h image.c image.h)
target_link_libraries(team08 PRIVATE ${JPEGLIB})