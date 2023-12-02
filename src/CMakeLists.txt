cmake_minimum_required(VERSION 3.24)

project(NodeBot VERSION 0.0.1 LANGUAGES CXX)
set(NAMESPACE ok)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source!")
endif()

file(WRITE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h.in "
#pragma once

#define ${PROJECT_NAME}_VERSION @${PROJECT_NAME}_VERSION@
#define ${PROJECT_NAME}_VERSION_MAJOR @${PROJECT_NAME}_VERSION_MAJOR@
#define ${PROJECT_NAME}_VERSION_MINOR @${PROJECT_NAME}_VERSION_MINOR@
#define ${PROJECT_NAME}_VERSION_PATCH @${PROJECT_NAME}_VERSION_PATCH@
")

# configure a header file to pass the version number only
configure_file(${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h.in ${PROJECT_NAME}Config.h)

include(CPack)
include(CMakeDependentOption)
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)
include(CMakePackageConfigHelpers)

find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)
find_package(toml11 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(mosquitto REQUIRED)

# define library
add_library(${PROJECT_NAME}
    include/NodeBot/NodeBot.hpp
    include/NodeBot/DataTypes.hpp
    Config.hpp
    NodeBot.cpp
        control/ChannelController.cpp
        control/ChannelController.hpp
        control/DppController.cpp
        control/DppController.hpp
        control/NodeController.cpp
        control/NodeController.hpp
        control/SlashCommandController.cpp
        control/SlashCommandController.hpp
        control/NodeQueues.cpp
        control/NodeQueues.hpp
        control/InternalDataTypes.hpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Threads::Threads
    spdlog::spdlog
    toml11::toml11
    nlohmann_json::nlohmann_json
    mosquitto::libmosquittopp
    dpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${toml_SOURCE_DIR}
    ${dpp_SOURCE_DIR}/include
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

# add compiler warning flags just when building this project via the BUILD_INTERFACE
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
target_compile_options(${PROJECT_NAME} PUBLIC
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wpedantic;-Werror;-Wformat=2;-Wunused;-Wno-redundant-move>>"
)

# state that the library need PIC when the default is shared libraries
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
)

add_library(${NAMESPACE}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# installation
if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH ${ORIGIN})
endif()

if(NOT CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR "bin")
endif()
if(NOT CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()
if(NOT CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR "include")
endif()

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/${PROJECT_NAME}
        DESTINATION include
        COMPONENT Devel
)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h"
        DESTINATION include/${PROJECT_NAME}
)

# setup installer
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
set(CPACK_SOURCE_GENERATOR "TGZ")

# install the configuration targets
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME} CACHE PATH "Location of header files" )
set(SYSCONFIG_INSTALL_DIR ${CMAKE_INSTALL_SYSCONFDIR}/${CMAKE_PROJECT_NAME} CACHE PATH "Location of configuration files" )

# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# generate the version file for the config file
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${Tutorial_VERSION_MAJOR}.${Tutorial_VERSION_MINOR}.${Tutorial_VERSION_PATCH}
        COMPATIBILITY SameMajorVersion
)

# install configuration files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(EXPORT ${CMAKE_PROJECT_NAME}Targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        COMPONENT Devel
)
