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
        spdlog::spdlog
)

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
