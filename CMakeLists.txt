cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Новая цель только для проверки шаблонов (Lab 6) ---
add_executable(lab6_templates
    src/main.cpp
)
target_include_directories(lab6_templates PRIVATE include)
target_compile_options(lab6_templates PRIVATE -Wall -Wextra -Wpedantic)

# --- Старая цель (если хочешь продолжать использовать старые файлы) ---
# add_executable(dispatcher_trainer
#     src/main.cpp
#     src/DispatchUnit.cpp
#     src/Incident.cpp
#     src/EventLog.cpp
#     src/DispatchCenter.cpp
#     # ... остальные старые файлы
# )
# target_include_directories(dispatcher_trainer PRIVATE include)
# target_compile_options(dispatcher_trainer PRIVATE -Wall -Wextra -Wpedantic)
# target_sources(dispatcher_trainer PRIVATE
#     src/model/Signal.cpp
#     src/model/Train.cpp
#     src/model/Station.cpp
#     src/model/Route.cpp
#     src/model/Incident.cpp
#     src/model/ARHint.cpp
#     src/model/DispatcherModel.cpp
#     src/visual/ConsoleVisualRenderer.cpp
#     src/dispatcher/Utils.cpp
# )

# ─── Тесты (Lab 6) ───
enable_testing()

add_executable(test_template_rules
    tests/test_template_rules.cpp
)
target_include_directories(test_template_rules PRIVATE include)
add_test(NAME template_rules COMMAND test_template_rules)