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