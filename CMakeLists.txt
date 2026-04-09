cmake_minimum_required(VERSION 3.24)
project("room-lights" VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    jansson
    GIT_REPOSITORY https://github.com/akheron/jansson.git
    GIT_TAG master
)

FetchContent_MakeAvailable(jansson)

add_executable(${PROJECT_NAME}
    src/inactivity_timer.cpp
    src/bulb.cpp
    src/udp.cpp
    src/controller.cpp
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE jansson)

option(ENABLE_TRACING "Enable tracing output" OFF)

if(ENABLE_TRACING)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TRACING=1)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_NAME_STR="${PROJECT_NAME}")
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
