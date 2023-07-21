cmake_minimum_required(VERSION 3.0.0)

project(hashmap C)

add_library(object_files OBJECT "misc.c" "hashmap.c")
target_compile_options(object_files PRIVATE "-Wall" "-O2" "-g" "-pedantic" "-Wextra" "-Wconversion")

add_library("hashmap-shared" SHARED $<TARGET_OBJECTS:object_files>)
add_library("hashmap-static" STATIC $<TARGET_OBJECTS:object_files>)
