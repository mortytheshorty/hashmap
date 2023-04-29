cmake_minimum_required(VERSION "3.14.1")

project("hashmap")

set(INCLUDE_PATH "./include")
set(SOURCE_PATH "./src")

set(${PROJECT_NAME}_HEADERS "${INCLUDE_PATH}/string.h")

add_library(${PROJECT_NAME} SHARED)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_PATH})

target_sources(${PROJECT_NAME} PRIVATE "${SOURCE_PATH}/hashmap_internal.c" "${SOURCE_PATH}/hashmap.c" "${SOURCE_PATH}/misc.c")

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-O2" "-pedantic" "-Wextra" "-Wconversion")


