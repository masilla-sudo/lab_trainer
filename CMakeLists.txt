cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Сначала объявляем исполняемый файл (цель)
add_executable(dispatcher_trainer
    src/main.cpp
    # Старые файлы из Lab 4 (оставь, если они нужны)
    src/DispatchUnit.cpp
    src/Incident.cpp
    src/EventLog.cpp
    src/DispatchCenter.cpp
)

# Теперь можно задавать свойства для этой цели
target_include_directories(dispatcher_trainer PRIVATE include)

# Новые файлы для Lab 5
target_sources(dispatcher_trainer PRIVATE
    src/model/Signal.cpp
    src/model/Train.cpp
    src/model/Station.cpp
    src/model/Route.cpp
    src/model/Incident.cpp
    src/model/ARHint.cpp
    src/model/DispatcherModel.cpp
    src/visual/ConsoleVisualRenderer.cpp
    src/dispatcher/Utils.cpp
)

target_compile_options(dispatcher_trainer PRIVATE -Wall -Wextra -Wpedantic)

# ─── Тесты (Lab 6) ───
enable_testing()

add_executable(test_template_rules
    tests/test_template_rules.cpp
)
target_include_directories(test_template_rules PRIVATE include)

add_test(NAME template_rules COMMAND test_template_rules)