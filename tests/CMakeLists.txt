cmake_minimum_required(VERSION 3.24)

project(NodeBotTests)

enable_testing()

find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)

add_executable(${PROJECT_NAME}
        control/DppControllerTests.cpp
        control/NodeQueuesTests.cpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Threads::Threads
        spdlog::spdlog
        dpp
        ok::NodeBot
        GTest::gtest_main
)

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${toml_SOURCE_DIR}
        ${dpp_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
