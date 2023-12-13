cmake_minimum_required(VERSION 3.26)
project(ScalaVector)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)

find_package(Catch REQUIRED)

add_executable(ScalaVector immutable_vector.h
        test/test.cpp)
target_link_libraries(ScalaVector PRIVATE contrib_catch_main)