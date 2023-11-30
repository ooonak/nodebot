cmake_minimum_required(VERSION 3.24)

project(NodeBotApp)

# dependencies
find_package(spdlog REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ok::NodeBot
)
