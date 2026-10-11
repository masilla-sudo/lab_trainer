# dispatcher_trainer — консольный тренажёр диспетчера

Цель работы: применить современные возможности C++ (C++17/20) для безопасной обработки состояний и команд в ядре диспетчерского тренажёра.

## Реализованные технологии

- **`std::optional`** — безопасный поиск параметров без «магических» значений вроде `-1`. Если параметр не найден, возвращается `std::nullopt`.
- **`std::variant`** — строгая типизация результатов команд: `CommandOk`, `CommandWarning`, `CommandError`. `CommandError` возвращается только при ненайденном параметре; выход за диапазон даёт `CommandWarning` (событие `Alarm`). Вывод сообщения через `std::visit`.
- **`std::filesystem`** — сохранение журнала событий в файл `logs/dispatch_session.txt`.

---

## Проверяемые сценарии (пороговая логика)

| Сценарий | Значение | Ожидаемый результат | Логика порога |
|----------|----------|---------------------|---------------|
| Норма | 60.0 | Команда принята | В пределах [0..80] |
| Критическая зона (2%) | 79.0 | Критическая близость границы | 80 − (80 × 0.02) = 78.4 |
| Зона предупреждения (5%) | 77.0 | Предупреждение | 80 − (80 × 0.05) = 76.0 |
| Вне диапазона | 100.0 | Предупреждение (CommandWarning), событие Alarm | > 80 |

---

## Основные команды консольного меню

- `find <name>` — поиск параметра (использует `std::optional`).
- `set <param> <val>` — изменение параметра (результат в `std::variant`).
- `save` — сохранение журнала (через `std::filesystem`).
- `log` — просмотр журнала событий.

---

## Примеры работы (вывод в консоль)

**Поиск параметра:**

```text
find V-101
Найден: V-101 = 60 км/ч [0..80]

find NOPE
Параметр "NOPE" не найден.
```

**Изменение параметра:**

```text
set V-101 70.0
Команда принята: V-101 = 70.000000

set V-101 90.0
Значение вне диапазона [0.000000..80.000000]

set V-101 90.0
Вне диапазона (90.0): Значение вне диапазона [0.000000..80.000000]
```

**Сохранение журнала:**

```text
save
Журнал сохранён: logs/dispatch_session.txt
```

---

## Сборка и запуск

### Основной способ: devcontainer (VS Code)

1. Установить Docker и VS Code.
2. Установить расширение **Dev Containers**.
3. Открыть папку проекта в VS Code.
4. Выполнить команду **Reopen in Container**.
5. Дождаться сборки контейнера, открыть встроенный терминал и выполнить:

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
./build-debug/dispatcher_trainer
```

### Альтернативный способ: Linux VM (VirtualBox)

В виртуальной машине Linux инструменты устанавливаются напрямую, а не через Dockerfile — среда зависит от того, что установлено в самой VM.

```bash
sudo apt update
sudo apt install -y build-essential cmake git gdb
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
./build-debug/dispatcher_trainer
```

> Базовый devcontainer-образ Microsoft (`mcr.microsoft.com/devcontainers/cpp:1-debian-12`) выбран вместо голого `ubuntu:24.04` из-за ограничений сети: образ кешируется целиком, а список устанавливаемых пакетов зафиксирован в Dockerfile, поэтому воспроизводимость среды сохраняется.


## Диаграмма 1: Поток данных

```mermaid

flowchart TD
    A[Parameter] --> B[Range]
    B --> C[RangeRule]
    C --> D[ICheckRule]
    D --> E[CheckResult]
    E --> F[VisualDescriptor]
    A --> F
    C --> E
```

## Диаграмма 2: Классы правил и проверок

```mermaid

classDiagram
    class ICheckRule {
        <<interface>>
        +check() CheckResult
    }
    class RangeRule {
        -parameter : Parameter
        -normalRange : Range
        -criticalRange : optional~Range~
        -problemMessage : string
        +check() CheckResult
    }
    class Parameter {
        -id : string
        -label : string
        -value : double
        -unit : string
        +id() string
        +label() string
        +unit() string
        +value() double
        +setValue(double) void
    }
    class Range {
        -min : double
        -max : double
        +contains(double) bool
    }
    class CheckResult {
        +parameterId : string
        +message : string
        +severity : Severity
    }
    class VisualDescriptor {
        +objectId : string
        +visualKind : string
        +label : string
        +color : string
        +priority : int
    }

    RangeRule ..|> ICheckRule : реализует
    RangeRule --> Parameter : использует
    RangeRule --> Range : содержит
    RangeRule ..> CheckResult : возвращает
    VisualDescriptor ..> CheckResult : формируется из

```

## Диаграмма 3: Ключевой фрагмент для ЛР 8
> Упрощённая схема уровня ЛР 8: здесь пороговая логика ещё не выделена
> в ядро `dispatcher_core`, поэтому `RangeRule` возвращает `CommandResult`.
> Актуальная архитектура (после ЛР 10) показана в Диаграмме 2 — там
> `check()` без аргументов возвращает `CheckResult`.

```mermaid
classDiagram
    class Parameter {
        +name: string
        +value: double
        +minValue: double
        +maxValue: double
        +unit: string
    }
    class RangeRule {
        -range: Range
        -criticalPercent: double
        -warningPercent: double
        +check(value: double): CommandResult
    }
    class ICheckRule {
        <<interface>>
        +check(value: double): CommandResult
    }

    Parameter "1" *-- "1" RangeRule : проверяется через
    RangeRule ..|> ICheckRule : реализует

```

---

## Изменения в ЛР 9

- оформлена история Git;
- создана рабочая ветка;
- выполнено слияние;
- разобран простой конфликт;
- уточнён демонстрационный сценарий.

## Изменения в ЛР 10

- исходники ядра перенесены в `src/core` (`model.cpp`, `log.cpp`, `commands.cpp`), консольное приложение — в `src/app`;
- проект разделён на статическую библиотеку `dispatcher_core` и приложение `dispatcher_trainer`;
- стандарт C++20 задан через `target_compile_features` на уровне цели;
- сборка ведётся в двух конфигурациях: `build-debug` и `build-release` (out-of-source);
- тесты линкуются с библиотечным ядром вместо пересборки исходников;
- проверена идентичность вывода приложения в Debug и Release.

## Изменения в ЛР 11

- добавлен каталог `.devcontainer` с `Dockerfile` и `devcontainer.json` — проект открывается в VS Code внутри контейнера (**Dev Containers: Reopen in Container**);
- базовый образ — `mcr.microsoft.com/devcontainers/cpp:1-debian-12`; в контейнер устанавливаются cmake, ninja-build, git, clang, clangd, clang-format, lldb, gdb, pkg-config, ca-certificates;
- версия образа и список пакетов зафиксированы в репозитории — среда сборки воспроизводима и не зависит от конфигурации компьютера;
- сборка и запуск выполняются из терминала внутри контейнера: `cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug`, `cmake --build build-debug`, `./build-debug/dispatcher_trainer`;
- `.gitignore` исключает каталоги сборки (`build/`, `build-debug/`, `build-release/`, `cmake-build-*/`) — исходники отделены от артефактов;
- изменения зафиксированы коммитом `Add devcontainer for reproducible C++ environment`.

## Ограничения первого модуля

Проект остаётся локальным консольным тренажёром. JSON/INI, сеть, GUI, процессы, потоки, устройства и CI/CD в рамках работы не используются.

## Запуск тестов

Тесты собираются вместе с проектом и запускаются через CTest:

    cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
    cmake --build build-debug
    ctest --test-dir build-debug --output-on-failure

Или напрямую:

    ./build-debug/test_core            # тесты ядра (ЛР 12)
    ./build-debug/test_template_rules  # тесты пороговых правил

## Изменения в ЛР 12

- добавлен `tests/test_core.cpp` — минимальный модульный тестовый фреймворк
  (функция `check`, подход xUnit: независимые тест-функции, Arrange/Act/Assert);
- покрыты 4 группы: граничные проверки `Range` и шаблона `isInsideRange`,
  изменение состояния через `setParameter`/`applyCommand`, поиск параметра
  (`std::optional`), журнал событий (`addEvent`, статистика);
- тесты зарегистрированы в CMake как цель `test_core` и добавлены в CTest;
- тесты зафиксировали фактический контракт ядра: `setParameter` не проверяет
  диапазон (валидация инкапсулирована в `applyCommand`, результат — variant
  `CommandOk`/`CommandWarning`/`CommandError`);
- продемонстрировано падение теста при намеренно испорченном ожидании
  (значение 80.1 чуть выше верхней границы диапазона) и его исправление.
