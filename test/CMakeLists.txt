project(NodeBotUnitTests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    nb/ControllerUnitTests.cpp
    nb/NodeQueuesUnitTests.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/dpp
    ${googletest_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  NodeBot::NodeBot
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
