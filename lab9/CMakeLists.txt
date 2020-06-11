cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 11)

# Lab name
set(LAB_NAME "lab9")

# Lab tasks
#list(APPEND SOURCE_FILES
#        task1.c
#        task2.c
#        mytail.c
#        mystat.c
#        myls.c
#        task6.c
#    )
list(APPEND NON_COMPILABLE_SRC
        .execme
    )

### Here goes the template

project("${LAB_NAME}" C)

add_custom_target("${LAB_NAME}")

#foreach (file IN LISTS SOURCE_FILES)
#    add_executable("${LAB_NAME}_${file}_run" "${file}")
#    add_dependencies("${LAB_NAME}" "${LAB_NAME}_${file}_run")
#endforeach ()

find_package (Threads)
add_executable(${LAB_NAME}_main-race_run main-race.c mythread.h)
add_executable(${LAB_NAME}_main-deadlock_run main-deadlock.c mythread.h)
add_executable(${LAB_NAME}_main-deadlock-global_run main-deadlock-global.c mythread.h)
add_executable(${LAB_NAME}_main-signal_run main-signal.c mythread.h)
add_executable(${LAB_NAME}_main-signal-cv_run main-signal-cv.c mythread.h)
target_link_libraries (${LAB_NAME}_main-race_run ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (${LAB_NAME}_main-deadlock_run ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (${LAB_NAME}_main-deadlock-global_run ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (${LAB_NAME}_main-signal_run ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (${LAB_NAME}_main-signal-cv_run ${CMAKE_THREAD_LIBS_INIT})

add_dependencies("${LAB_NAME}" ${LAB_NAME}_main-race_run)
add_dependencies("${LAB_NAME}" ${LAB_NAME}_main-deadlock_run)
add_dependencies("${LAB_NAME}" ${LAB_NAME}_main-deadlock-global_run)
add_dependencies("${LAB_NAME}" ${LAB_NAME}_main-signal_run)
add_dependencies("${LAB_NAME}" ${LAB_NAME}_main-signal-cv_run)

foreach (file IN LISTS NON_COMPILABLE_SRC)
    add_custom_command(
            TARGET "${LAB_NAME}" POST_BUILD
            DEPENDS "${file}"
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/${file}"
            "${CMAKE_CURRENT_BINARY_DIR}/${file}"
    )
endforeach ()