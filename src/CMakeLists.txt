cmake_minimum_required(VERSION 3.20)

project(scheduler VERSION 0.1.0 LANGUAGES CXX)
find_package(Boost REQUIRED)

add_executable(scheduler main.cpp)
target_link_libraries(scheduler ${Boost_LIBRARIES})

set(cmake_export_compile_commands 1)
