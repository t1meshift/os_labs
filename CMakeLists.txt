cmake_minimum_required(VERSION 3.16)
project(os_labs C)

set(CMAKE_C_STANDARD 11)

function(define_lab lab_name)
    add_subdirectory("${lab_name}")
    add_custom_target("RUN_${lab_name}" ALL
        "${PROJECT_SOURCE_DIR}/scripts/run_all.sh" "${PROJECT_BINARY_DIR}/${lab_name}"
        USES_TERMINAL
        DEPENDS "${lab_name}"
    )
    message("${lab_name} defined.")
endfunction()

define_lab(lab2)
define_lab(lab3)
define_lab(lab4)
define_lab(lab5)
define_lab(lab6)
define_lab(lab7)
define_lab(lab8)
define_lab(lab9)
define_lab(lab10)
define_lab(lab12)