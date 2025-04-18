cmake_minimum_required(VERSION 3.10)
project(glowing_umbrella C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(glowing_umbrella src/main.c)