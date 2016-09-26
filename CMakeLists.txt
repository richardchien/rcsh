cmake_minimum_required(VERSION 3.6)
project(rcsh)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES main.c CallbackMap.c CallbackMap.h CLIMenu.c CLIMenu.h Callback.h)
add_executable(rcsh ${SOURCE_FILES})