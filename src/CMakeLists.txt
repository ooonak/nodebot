cmake_minimum_required(VERSION 3.22)

project(NodeBot VERSION 0.0.1)

file(WRITE ${PROJECT_BINARY_DIR}/nodebotconfig.hpp.in "
#pragma once

#define NodeBot_VERSION_MAJOR @NodeBot_VERSION_MAJOR@
#define NodeBot_VERSION_MINOR @NodeBot_VERSION_MINOR@
#define NodeBot_VERSION_PATCH @NodeBot_VERSION_PATCH@
")

find_package(Threads REQUIRED)

add_library(${PROJECT_NAME}
    include/nb/NodeBot.hpp
    include/nb/NodeHandle.hpp
    Config.hpp
    NodeBot.cpp
    NodeHandle.cpp
    dpp/ChannelController.cpp
    dpp/ChannelController.hpp
    dpp/DppController.cpp
    dpp/DppController.hpp
    dpp/NodeController.cpp
    dpp/NodeController.hpp
    dpp/SlashCommandController.cpp
    dpp/SlashCommandController.hpp
    dpp/WebHookController.cpp
    dpp/WebHookController.hpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
    ${spdlog_SOURCE_DIR}/include
    ${toml_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Threads::Threads
    spdlog::spdlog
    toml11::toml11
    nlohmann_json::nlohmann_json
    dpp
)

add_library(NodeBot::NodeBot ALIAS NodeBot)

configure_file("${PROJECT_BINARY_DIR}/nodebotconfig.hpp.in" nodebotconfig.hpp)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/nb
        DESTINATION include
        COMPONENT Devel
)

