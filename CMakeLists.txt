cmake_minimum_required(VERSION 3.20)
project(ccl)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    message(FATAL_ERROR "Please into another dir to build!")
endif()

if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    message(FATAL_ERROR "Please into another dir to build!")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "build for release version")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "build for debug version")
else()
    message(STATUS "build for unknown version")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(UNIX_OS true)
    add_definitions("-O2 -g")
    add_definitions("-Wno-invalid-source-encoding")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(UNIX_OS true)
    add_definitions("-flto")
    add_definitions("-Os -g")
    add_definitions("-Wno-invalid-source-encoding")
    add_definitions("-fdata-sections -ffunction-sections")
    add_definitions("-fno-rtti -fno-exceptions")
    if(OPTMIZE_SIZE MATCHES "YES")
        add_definitions("-fvisibility=hidden -fvisibility-inlines-hidden")
    endif()
else()
    message(FATAL_ERROR "unknown CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
endif()

add_definitions(
        "-W"
        "-Wall"
        "-Werror"
        "-Wshadow"
        "-Wformat"
        "-Wpointer-arith"
        "-D_REENTRANT"
        "-Wno-long-long"
        "-Wuninitialized"
        "-D_POSIX_PTHREAD_SEMANTICS"
        "-fexceptions"
        "-fPIC"
        # "-Wno-unused-parameter"
        # "-fsanitize=address"
        # "-fno-omit-frame-pointer"
)

set(INCLUDE
        include
        include/aio
        include/pipe
        include/types)

file(GLOB SOURCE
        include/*.h
        include/aio/*.h
        include/pipe/*.h
        include/types/*.h
        src/*.cpp
        src/common/*.cpp
        src/io/*.cpp)

include_directories(${INCLUDE})
add_library(ccl ${SOURCE})
