cmake_minimum_required(VERSION 3.20)
project(dispatcher_trainer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(dispatcher_trainer
src/main.cpp
src/trainer_logic.cpp
)
target_include_directories(dispatcher_trainer PRIVATE include)
target_compile_options(dispatcher_trainer PRIVATE -Wall -Wextra -Wpedantic)