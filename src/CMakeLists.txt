
find_library(JPEGLIB REQUIRED NAMES libjpeg.a PATHS "usr/local/lib")

add_executable(team08
        main.c load_image.c
        matrix.c matrix.h image.c image.h timing.c timing.h
        baseline/calc_errors.c
        baseline/min_cut.c
        baseline/quilting.c

        opt_1/calc_errors_opt.c
        opt_1/quilting_opt.c
        opt_1/min_cut_opt.c
        )
target_link_libraries(team08 PRIVATE ${JPEGLIB})


