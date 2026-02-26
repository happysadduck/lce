cmake_minimum_required(VERSION 3.11)
project(lce C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

file(GLOB SOURCES "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} raylib)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})