
find_library(JPEGLIB REQUIRED NAMES libjpeg.a PATHS "usr/local/lib")
add_executable(team08
        main.c load_image.c calc_errors.c quilting.c quilting.c)
target_link_libraries(team08 PRIVATE ${JPEGLIB})