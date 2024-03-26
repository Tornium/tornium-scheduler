cmake_minimum_required(VERSION 3.10)

project(scheduler)
find_package(Boost REQUIRED)

add_executable(scheduler main.cpp)
target_link_libraries(scheduler ${Boost_LIBRARIES})
