cmake_minimum_required(VERSION 3.18)
project(design_pattern)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Werror")

add_executable(strategy strategy.cpp)
add_executable(builder builder.cpp)
add_executable(factory factory.cpp)
add_executable(singleton singleton.cpp)
add_executable(adapter adapter.cpp)
add_executable(command command.cpp)
add_executable(chain chain.cpp)