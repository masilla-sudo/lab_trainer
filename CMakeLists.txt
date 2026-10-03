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

# --- Тесты ядра (Lab 12) ---
add_executable(test_core
    tests/test_core.cpp
)
target_link_libraries(test_core
    PRIVATE
        dispatcher_core
)
target_compile_features(test_core
    PRIVATE
        cxx_std_20
)
target_compile_options(test_core PRIVATE -Wall -Wextra -Wpedantic)
add_test(NAME core COMMAND test_core)

# --- ЛР 13: опциональная сборка с санитайзерами ---
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

function(enable_project_sanitizers target_name)
    if(ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target_name} PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
            -g
        )
        target_link_options(${target_name} PRIVATE
            -fsanitize=address,undefined
        )
    endif()
endfunction()

enable_project_sanitizers(dispatcher_core)
enable_project_sanitizers(dispatcher_trainer)
enable_project_sanitizers(test_core)
enable_project_sanitizers(test_template_rules)
