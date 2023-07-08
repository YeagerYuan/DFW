cmake_minimum_required(VERSION 3.5)
project(rich)
set(CMAKE_CXX_STANDARD 11)
SET(EXECUTABLE_OUTPUT_PATH ".")

set(SOURCE_FILES
    ../src/base.h
    ../src/display.h
    ../src/display.c
    ../src/input.h
    ../src/input.c
    ../src/judge.h
    ../src/play.h
    ../src/play.c
    ../src/stat_judge.c
    ../src/new_test.h
    ../src/new_test.c
    )

add_executable(test ${SOURCE_FILES})