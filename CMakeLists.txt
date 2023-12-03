cmake_minimum_required(VERSION 3.24)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source!")
endif()

project(NodeBot)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(TESTING "Build tests" ON)

include(dependencies.cmake)
EnableUseFetchContent()

add_subdirectory(src)
add_subdirectory(app)

if(TESTING)
    include(CTest)
    add_subdirectory(tests)
endif(TESTING)

