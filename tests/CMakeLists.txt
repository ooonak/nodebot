cmake_minimum_required(VERSION 3.24)

project(NodeBotTests)

enable_testing()

add_executable(${PROJECT_NAME}
        ControllerUnitTests.cpp
        NodeQueuesUnitTests.cpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ok::NodeBot
        GTest::gtest_main
)

#target_include_directories(${PROJECT_NAME}
#    PRIVATE
#    ${CMAKE_SOURCE_DIR}/src
#    ${CMAKE_SOURCE_DIR}/src/dpp
#    ${googletest_SOURCE_DIR}
#)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
