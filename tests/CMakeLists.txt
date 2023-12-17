cmake_minimum_required(VERSION 3.24)

project(NodeBotTests)

enable_testing()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)
find_package(toml11 REQUIRED)

# pkg_check_module works with Nix package
find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquittopp REQUIRED)

add_executable(${PROJECT_NAME}
        #MQTTControllerTests.cpp
        #MQTTClientTests.cpp
        control/DppControllerTests.cpp
        control/NodeQueuesTests.cpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Threads::Threads
        spdlog::spdlog
        toml11::toml11
        dpp
        PkgConfig::Mosquitto # Works with Nix
        ok::NodeBot
        GTest::gtest_main
)

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${toml11_INCLUDE_DIR}
        #${dpp_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR/src}
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
