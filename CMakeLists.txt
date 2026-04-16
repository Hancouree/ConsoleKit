cmake_minimum_required(VERSION 3.10)
project(ConsoleKit VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_executable(demo examples/main.cpp)
target_link_libraries(demo PRIVATE ${PROJECT_NAME})