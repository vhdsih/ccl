cmake_minimum_required(VERSION 3.20)
project(ccl)

set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS_DEBUG "-O0")    # 调试包不优化
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG") # release 包优化

message("-- Build type is " ${CMAKE_BUILD_TYPE})

add_definitions(
        "-W"
        "-Wall"
        "-Werror"
        "-Wextra"
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
        ${INCLUDE_DIR}/utils
        ${WIREDTIGER_DIR}/build/include
        )

file(GLOB SOURCE_FILE
        ${SOURCE_DIR}/*.cpp
        ${SOURCE_DIR}/aio/*.cpp
        ${SOURCE_DIR}/db/*.cpp
        ${SOURCE_DIR}/db/wiredtiger/*.cpp
        ${SOURCE_DIR}/common/*.cpp
        )

set(EXT_LIBS
    ${THIRD_PARTY}/wiredtiger/build/libwiredtiger.dylib
)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${INCLUDE})
link_directories(${WIREDTIGER_DIR}/build/)

add_library(${PROJECT_NAME} ${SOURCE_FILE})
target_link_libraries(${PROJECT_NAME} pthread)


# example
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/examples)
add_executable(run_lib ${INCLUDE} examples/run_lib.cpp)
target_link_libraries(run_lib ${PROJECT_NAME} wiredtiger)

add_executable(db_wt ${INCLUDE} examples/db/wt.cpp)
target_link_libraries(db_wt ${PROJECT_NAME} wiredtiger)
