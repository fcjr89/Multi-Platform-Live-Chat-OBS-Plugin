cmake_minimum_required(VERSION 3.16...3.26)

project(multi-platform-chat VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network WebSockets)

# Find OBS
if(WIN32)
    if(DEFINED ENV{LIBOBS_INCLUDE_DIR})
        set(LIBOBS_INCLUDE_DIR $ENV{LIBOBS_INCLUDE_DIR})
    endif()
    
    if(NOT LIBOBS_INCLUDE_DIR)
        message(FATAL_ERROR "LIBOBS_INCLUDE_DIR not set")
    endif()
    
    # Set OBS library paths for Windows
    set(OBS_LIB_DIR "${LIBOBS_INCLUDE_DIR}/../bin/64bit")
    
    message(STATUS "OBS Include Dir: ${LIBOBS_INCLUDE_DIR}")
    message(STATUS "OBS Lib Dir: ${OBS_LIB_DIR}")
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBOBS REQUIRED libobs)
endif()

# Source files
set(PLUGIN_SOURCES
    src/plugin-main.cpp
    src/chat-aggregator.cpp
    src/chat-dock.cpp
    src/websocket-server.cpp
    src/browser-source.cpp
)

# Header files
set(PLUGIN_HEADERS
    src/platform-handler.hpp
    src/chat-aggregator.hpp
    src/chat-dock.hpp
    src/websocket-server.hpp
    src/browser-source.hpp
)

# Add all platform handlers
file(GLOB PLATFORM_HANDLER_SOURCES "src/platform-handlers/*.cpp")
file(GLOB PLATFORM_HANDLER_HEADERS "src/platform-handlers/*.hpp")

list(APPEND PLUGIN_SOURCES ${PLATFORM_HANDLER_SOURCES})
list(APPEND PLUGIN_HEADERS ${PLATFORM_HANDLER_HEADERS})

# Create the plugin as a MODULE (DLL)
add_library(${PROJECT_NAME} MODULE
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::WebSockets
)

# Link OBS libraries
if(WIN32)
    # Link to OBS DLL via import library (created at runtime)
    target_link_options(${PROJECT_NAME} PRIVATE 
        "/DELAYLOAD:obs.dll"
        "/DELAYLOAD:obs-frontend-api.dll"
    )
    target_link_libraries(${PROJECT_NAME}
        delayimp.lib
    )
else()
    target_link_libraries(${PROJECT_NAME}
        ${LIBOBS_LIBRARIES}
    )
endif()

# Include OBS headers (both libobs and UI)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LIBOBS_INCLUDE_DIR}
    ${LIBOBS_INCLUDE_DIR}/../UI/obs-frontend-api
)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "multi-platform-chat"
)

message(STATUS "===========================================")
message(STATUS "Multi-Platform Chat OBS Plugin")
message(STATUS "===========================================")
message(STATUS "Qt6 location: ${Qt6_DIR}")
message(STATUS "OBS headers: ${LIBOBS_INCLUDE_DIR}")
message(STATUS "Source files found: ${PLUGIN_SOURCES}")
message(STATUS "===========================================")
