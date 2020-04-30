cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 11)

# Lab name
set(LAB_NAME "lab4")

# Lab tasks
list(APPEND SOURCE_FILES
        aspace.c
        task2.c
        null.c
        task6.c
        task7.c
        task8.c
    )
list(APPEND NON_COMPILABLE_SRC
#        .execme
    )

set_source_files_properties(aspace.c PROPERTIES COMPILE_FLAGS -O0)
#set_source_files_properties(null.c PROPERTIES COMPILE_FLAGS -DHANDLE_SIGSEGV)
set_source_files_properties(null.c PROPERTIES COMPILE_FLAGS -g)
set_source_files_properties(task6.c PROPERTIES COMPILE_FLAGS -g)
set_source_files_properties(task7.c PROPERTIES COMPILE_FLAGS -g)
set_source_files_properties(task8.c PROPERTIES COMPILE_FLAGS -g)

### Here goes the template

project("${LAB_NAME}" C)

add_custom_target("${LAB_NAME}")

foreach (file IN LISTS SOURCE_FILES)
    add_executable("${LAB_NAME}_${file}_run" "${file}")
    add_dependencies("${LAB_NAME}" "${LAB_NAME}_${file}_run")
endforeach ()

foreach (file IN LISTS NON_COMPILABLE_SRC)
    add_custom_command(
            TARGET "${LAB_NAME}" POST_BUILD
            DEPENDS "${file}"
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/${file}"
            "${CMAKE_CURRENT_BINARY_DIR}/${file}"
    )
endforeach ()