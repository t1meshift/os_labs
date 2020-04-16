cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 11)

# Lab name
set(LAB_NAME "lab2")

# Lab tasks
list(APPEND SOURCE_FILES
        task1.c
        task2.c
        task3.c
        task4.c
        task7.c
        task8.c
    )


### Here goes the template

project("${LAB_NAME}" C)

add_custom_target("${LAB_NAME}")

foreach (file IN LISTS SOURCE_FILES)
    add_executable("${LAB_NAME}_${file}_run" "${file}")
    add_dependencies("${LAB_NAME}" "${LAB_NAME}_${file}_run")
endforeach ()