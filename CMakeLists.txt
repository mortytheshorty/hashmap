cmake_minimum_required(VERSION 3.0.0)

project(hashmap C)

add_library(object_files OBJECT "hashmap.c" "hashmap.h")
target_compile_options(object_files PRIVATE "-Wall" "-O2" "-pedantic" "-Wextra" "-Wconversion")

add_library("hashmap-shared" SHARED $<TARGET_OBJECTS:object_files>)
add_library("hashmap-static" STATIC $<TARGET_OBJECTS:object_files>)


add_executable("simple_tests" "test/simple_tests.c")
target_link_libraries("simple_tests" "hashmap-static")