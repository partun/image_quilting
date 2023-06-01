
find_library(JPEGLIB REQUIRED NAMES libjpeg.so PATHS "usr/local/lib")


add_executable(team08
        main.c load_image.c
        matrix.c matrix.h image.c image.h timing.c timing.h
        baseline/calc_errors.c
        baseline/min_cut.c
        baseline/quilting.c

        opt_1/calc_errors_opt.c
        opt_1/quilting_opt.c
        opt_1/min_cut_opt.c

        opt_2/calc_errors_opt_2.c
        opt_2/quilting_opt_2.c
        opt_2/min_cut_opt_2.c

        opt_3/calc_errors_opt_3.c
        opt_3/quilting_opt_3.c
        opt_3/min_cut_opt_3.c

        opt_4/calc_errors_opt_4.c
        opt_4/quilting_opt_4.c
        opt_4/min_cut_opt_4.c

        opt_5/calc_errors_opt_5.c
        opt_5/quilting_opt_5.c
        opt_5/min_cut_opt_5.c

        opt_6/calc_errors_opt_6.c
        opt_6/quilting_opt_6.c
        opt_6/min_cut_opt_6.c

        opt_6a/calc_errors_opt_6a.c
        opt_6a/quilting_opt_6a.c
        opt_6a/min_cut_opt_6a.c

        opt_6b/calc_errors_opt_6b.c
        opt_6b/quilting_opt_6b.c
        opt_6b/min_cut_opt_6b.c

        opt_6c/calc_errors_opt_6c.c
        opt_6c/quilting_opt_6c.c
        opt_6c/min_cut_opt_6c.c

        opt_6d/calc_errors_opt_6d.c
        opt_6d/quilting_opt_6d.c
        opt_6d/min_cut_opt_6d.c

        opt_7/calc_errors_opt_7.c
        opt_7/quilting_opt_7.c
        opt_7/min_cut_opt_7.c

        opt_8/calc_errors_opt_8.c
        opt_8/quilting_opt_8.c
        opt_8/min_cut_opt_8.c

        opt_9/calc_errors_opt_9.c
        opt_9/quilting_opt_9.c
        opt_9/min_cut_opt_9.c

        opt_10/calc_errors_opt_10.c
        opt_10/quilting_opt_10.c
        opt_10/min_cut_opt_10.c
        )
target_link_libraries(team08 PRIVATE ${JPEGLIB})


