cmake_minimum_required(VERSION 3.22)

project(NodeBot)

option(BUILD_TESTS "Build test programs" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
include(dependencies)
EnableUseFetchContent()

set(CMAKE_VERBOSE_MAKEFILE ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnull-dereference")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1")
endif()

message(STATUS "CXX flags: ${CMAKE_CXX_FLAGS}")

add_subdirectory(src)
add_subdirectory(app)

if(BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif(BUILD_TESTS)
