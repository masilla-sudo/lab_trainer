cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(dispatcher_trainer
src/main.cpp
)
target_compile_options(dispatcher_trainer PRIVATE
-Wall
-Wextra
-Wpedantic
)