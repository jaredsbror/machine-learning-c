cmake_minimum_required(VERSION 3.10)
project(glowing_umbrella_c C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(glowing_umbrella_c src/main.c)