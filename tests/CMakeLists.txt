cmake_minimum_required(VERSION 3.24)

project(NodeBotTests)

enable_testing()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)
find_package(toml11 REQUIRED)

add_executable(${PROJECT_NAME}
        control/DppControllerTests.cpp
        control/NodeQueuesTests.cpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Threads::Threads
        spdlog::spdlog
        toml11::toml11
        dpp
        ok::NodeBot
        GTest::gtest_main
)

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${toml11_INCLUDE_DIR}
        ${dpp_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
