cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(dispatcher_trainer
    src/main.cpp
    src/DispatchUnit.cpp
    src/Incident.cpp
    src/EventLog.cpp
    src/DispatchCenter.cpp
    src/DispatchTypes.cpp 
)

target_include_directories(dispatcher_trainer PRIVATE include)

# Важно: для std::filesystem в GCC может потребоваться эта строка
target_link_libraries(dispatcher_trainer PRIVATE stdc++fs)

target_compile_options(dispatcher_trainer PRIVATE -Wall -Wextra -Wpedantic)