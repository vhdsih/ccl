cmake_minimum_required (VERSION 3.16)
project(ccl)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS_DEBUG "-O0")    # 调试包不优化
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG") # release 包优化

message("-- Build type is " ${CMAKE_BUILD_TYPE})

add_definitions(
        "-W"
        "-Wall"
        "-Werror"
        "-Wextra"
        # "-Wshadow"
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
set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party)

set(THIRD_PARTY_LOCAL ${THIRD_PARTY_DIR}/local)
set(THIRD_PARTY_LIB_DIR ${THIRD_PARTY_LOCAL}/lib)
set(THIRD_PARTY_LIB64_DIR ${THIRD_PARTY_LOCAL}/lib64)
set(THIRD_PARTY_INCLUDE_DIR ${THIRD_PARTY_LOCAL}/include)

set(INCLUDE
        ${INCLUDE_DIR}
        ${INCLUDE_DIR}/aio
        ${INCLUDE_DIR}/db
        ${INCLUDE_DIR}/db/wiredtiger
        ${INCLUDE_DIR}/pipe
        ${INCLUDE_DIR}/types
        ${INCLUDE_DIR}/utils
        ${INCLUDE_DIR}/thread
        ${THIRD_PARTY_INCLUDE_DIR}
        )

file(GLOB SOURCE_FILE
        ${SOURCE_DIR}/*.cpp
        ${SOURCE_DIR}/aio/*.cpp
        ${SOURCE_DIR}/db/*.cpp
        ${SOURCE_DIR}/db/wiredtiger/*.cpp
        ${SOURCE_DIR}/common/*.cpp
        ${SOURCE_DIR}/thread/*.cpp
        ${SOURCE_DIR}/utils/*.cpp
        )

include_directories(${INCLUDE})
link_directories(${THIRD_PARTY_LIB_DIR})
link_directories(${THIRD_PARTY_LIB64_DIR})

set(CMAKE_PREFIX_PATH ${THIRD_PARTY_LOCAL})
set(CMAKE_MODULE_PATH ${THIRD_PARTY_LIB_DIR}/lib/cmake/glog)
set(CMAKE_MODULE_PATH ${THIRD_PARTY_LIB_DIR}/lib/cmake/gflags)
set(CMAKE_MODULE_PATH ${THIRD_PARTY_LIB_DIR}/lib64/cmake/glog)
set(CMAKE_MODULE_PATH ${THIRD_PARTY_LIB_DIR}/lib64/cmake/gflags)

find_package(glog REQUIRED)
find_package(gflags REQUIRED)

include_directories(${gflags_INCLUDE_DIRS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_link_libraries(${PROJECT_NAME} wiredtiger glog::glog ${gflags_LIBRARIES} pthread dl)

message("* build opt " ${CMAKE_CXX_FLAGS_RELEASE})

# example
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(GLOB_RECURSE EXAMPLES ${EXAMPLES_DIR}/*.cpp)

foreach(EXAMPLE ${EXAMPLES})
    get_filename_component(WHICH ${EXAMPLE} NAME_WE)
    add_executable(${WHICH} ${EXAMPLE})
    target_link_libraries(${WHICH} ${PROJECT_NAME})
endforeach()

# install
INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

INSTALL (DIRECTORY ${INCLUDE_DIR}/
        DESTINATION include/ccl
        FILES_MATCHING PATTERN "*.h*")

INSTALL (DIRECTORY ${THIRD_PARTY_INCLUDE_DIR}/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h*")

INSTALL (DIRECTORY ${THIRD_PARTY_LIB_DIR}/
        DESTINATION lib)

INSTALL (DIRECTORY ${THIRD_PARTY_LIB64_DIR}/
        DESTINATION lib64)
