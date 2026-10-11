cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer
    VERSION 0.1.0
    LANGUAGES CXX
)

# --- Библиотечное ядро тренажера: модель, журнал, команды ---
add_library(dispatcher_core
    src/core/model.cpp
    src/core/commands.cpp
    src/core/log.cpp
)
target_include_directories(dispatcher_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(dispatcher_core
    PUBLIC
        cxx_std_20
)
target_compile_options(dispatcher_core PRIVATE -Wall -Wextra -Wpedantic)

# --- Консольное приложение: меню и ввод-вывод ---
add_executable(dispatcher_trainer
    src/app/main.cpp
)
target_link_libraries(dispatcher_trainer
    PRIVATE
        dispatcher_core
)
target_compile_features(dispatcher_trainer
    PRIVATE
        cxx_std_20
)

# --- Тесты пороговых правил (Lab 6) ---
enable_testing()

add_executable(test_template_rules
    tests/test_template_rules.cpp
)
target_link_libraries(test_template_rules
    PRIVATE
        dispatcher_core
)
add_test(NAME template_rules COMMAND test_template_rules)