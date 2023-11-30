cmake_minimum_required(VERSION 3.24)

project(NodeBotTests)

enable_testing()

add_executable(${PROJECT_NAME}
        DppControllerTests.cpp
        NodeQueuesTests.cpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ok::NodeBot
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
