cmake_minimum_required(VERSION 3.14)
project(polycons)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 REQUIRED COMPONENTS graphics)

if (APPLE)
    add_compile_definitions(POINT_RATIO=2)
else ()
    add_compile_definitions(POINT_RATIO=1)
endif ()

add_executable(polycons main.cpp)

target_link_libraries(polycons sfml-graphics)