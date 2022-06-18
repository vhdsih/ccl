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

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party)

set(WIREDTIGER_DIR ${THIRD_PARTY_DIR}/wiredtiger)

set(INCLUDE
        ${INCLUDE_DIR}
        ${INCLUDE_DIR}/aio
        ${INCLUDE_DIR}/db
        ${INCLUDE_DIR}/db/wiredtiger
        ${INCLUDE_DIR}/pipe
        ${INCLUDE_DIR}/types
        ${WIREDTIGER_DIR}/build/include
        )

file(GLOB SOURCE_FILE
        ${INCLUDE_DIR}/*.h
        ${INCLUDE_DIR}/aio/*.h
        ${INCLUDE_DIR}/pipe/*.h
        ${INCLUDE_DIR}/types/*.h
        ${SOURCE_DIR}/*.cpp
        ${SOURCE_DIR}/aio/*.cpp
        ${SOURCE_DIR}/db/*.cpp
        ${SOURCE_DIR}/db/wiredtiger/*.cpp
        ${SOURCE_DIR}/common/*.cpp
        )

set(EXT_LIBS
    ${THIRD_PARTY}/wiredtiger/build/libwiredtiger.dylib
)
 
include_directories(${INCLUDE})
add_library(ccl ${SOURCE_FILE})

target_link_libraries(${PROJECT_NAME} pthread)
