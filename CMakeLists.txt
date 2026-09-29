cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Основная цель: диспетчерский тренажёр ---
add_executable(dispatcher_trainer
    src/main.cpp
    src/model.cpp
    src/log.cpp
    src/commands.cpp
)

target_include_directories(dispatcher_trainer PRIVATE include)
target_compile_options(dispatcher_trainer PRIVATE -Wall -Wextra -Wpedantic)

# ─── Тесты (Lab 6) ───
enable_testing()

add_executable(test_template_rules
    tests/test_template_rules.cpp
)
target_include_directories(test_template_rules PRIVATE include)
add_test(NAME template_rules COMMAND test_template_rules)