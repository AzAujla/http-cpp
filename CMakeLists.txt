cmake_minimum_required(VERSION 3.10)
project(http)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
add_subdirectory(src/app)
add_subdirectory(src/http)
add_subdirectory(src/utils)

add_executable(server src/main.cpp)
target_link_libraries(server PRIVATE app http utils Threads::Threads)
