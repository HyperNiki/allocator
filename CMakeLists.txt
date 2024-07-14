cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)

set(PROJECT_NAME "allocator")

project(
    ${PROJECT_NAME}
        VERSION 1.0.0
        LANGUAGES C
)

add_subdirectory(memmanagment)

add_executable(${PROJECT_NAME} main.c)	# Создает исполняемый файл с именем main

target_link_libraries(${PROJECT_NAME}
        memmanagment
        )