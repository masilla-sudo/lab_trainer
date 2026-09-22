cmake_minimum_required(VERSION 3.20)
project(dispatcher_trainer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(dispatcher_trainer
    src/model.cpp
    src/log.cpp
    src/commands.cpp
    app/main.cpp
)

target_include_directories(dispatcher_trainer PRIVATE include)