# dispatcher_trainer – Лабораторная работа 8: Modern C++ State

Цель работы: применить современные возможности C++ (C++17/20) для безопасной обработки состояний и команд в ядре диспетчерского тренажёра.

## Реализованные технологии

- **`std::optional`** — безопасный поиск параметров без «магических» значений вроде `-1`. Если параметр не найден, возвращается `std::nullopt`.
- **`std::variant`** — строгая типизация результатов команд: `CommandOk`, `CommandWarning`, `CommandError`. Вывод сообщения через `std::visit`.
- **`std::filesystem`** — сохранение журнала событий в файл `logs/dispatch_session.txt`.

---

## Проверяемые сценарии (пороговая логика)

| Сценарий | Значение | Ожидаемый результат | Логика порога |
|----------|----------|---------------------|---------------|
| Норма | 60.0 | Команда принята | В пределах [0..80] |
| Критическая зона (2%) | 79.0 | Критическая близость границы | 80 − (80 × 0.02) = 78.4 |
| Зона предупреждения (5%) | 77.0 | Предупреждение | 80 − (80 × 0.05) = 76.0 |
| Вне диапазона | 100.0 | Ошибка: значение вне диапазона | > 80 |

---

## Основные команды консольного меню

- `find <name>` — поиск параметра (использует `std::optional`).
- `set <param> <val>` — изменение параметра (результат в `std::variant`).
- `save` — сохранение журнала (через `std::filesystem`).
- `log` — просмотр журнала событий.
- `elements` — полиморфный вывод элементов модели (см. раздел «Полиморфная модель»).

---

## Полиморфная модель

Элементы системы построены на интерфейсе `ISystemElement` (чистые виртуальные методы `id()`, `kind()`, `status()`, `summary()`) и наследуются от него: `Train`, `Station`, `Signal` (а также `Route`, `Incident`, `ARHint`). Каждый элемент параллельно реализует интерфейс `IVisualDescriptorProvider` с методом `toVisualDescriptor()`.

Контейнер `DispatcherModel` хранит элементы как `std::vector<std::unique_ptr<ISystemElement>>`. Метод `printSummary()` обходит коллекцию и вызывает `summary()` через базовый указатель — статический тип один (`ISystemElement*`), динамический у каждого элемента свой. Метод `collectVisualDescriptors()` достаёт визуальные описания через `dynamic_cast` к `IVisualDescriptorProvider`.

Владение элементами — через `std::make_unique`, полиморфное удаление — через виртуальный деструктор интерфейса.

**Пример вывода команды `elements`** (один цикл — разные форматы строк):

```text
> elements
--- Сводка элементов системы ---
Поезд T-101 на маршруте "R-7" (статус: Normal)
Поезд T-102 на маршруте "R-3" (статус: Normal)
Станция Северная (S-1, платформ: 4, статус: Normal)
Станция Южная (S-2, платформ: 2, статус: Normal)
Сигнал S-42 (состояние: Normal)
Сигнал S-43 (состояние: Critical)
--- Визуальные дескрипторы ---
  T-101 | train_marker | Поезд T-101 (R-7) | blue | приоритет 2
  T-102 | train_marker | Поезд T-102 (R-3) | blue | приоритет 2
  S-1 | station_marker | Станция Северная | green | приоритет 1
  S-2 | station_marker | Станция Южная | green | приоритет 1
  S-42 | marker | Сигнал S-42 | green | приоритет 1
  S-43 | marker | Сигнал S-43 | red | приоритет 1
```

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
Команда принята: V-101 = 70

set V-101 90.0
Внимание: значение V-101 вне диапазона [0.000000..80.000000]
```

**Сохранение журнала:**

```text
save
Журнал сохранён: logs/dispatch_session.txt
```

---

## Сборка и запуск

### В devcontainer (VS Code)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/dispatcher_trainer
```

### В Linux VM

```bash
sudo apt update
sudo apt install -y build-essential cmake git
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/dispatcher_trainer
```

---

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

    ICheckRule ..|> RangeRule : реализует
    RangeRule --> Parameter : использует
    RangeRule --> Range : содержит
    RangeRule ..> CheckResult : возвращает
    VisualDescriptor ..> CheckResult : формируется из
```

## Диаграмма 3: Ключевой фрагмент для ЛР 8

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

## Диаграмма 4: Полиморфная модель элементов

```mermaid
classDiagram
    class ISystemElement {
        <<interface>>
        +id() string
        +kind() string
        +status() string
        +summary() string
    }
    class IVisualDescriptorProvider {
        <<interface>>
        +toVisualDescriptor() VisualDescriptor
    }
    class Train {
        -id_ : string
        -route_ : string
        +summary() string
        +toVisualDescriptor() VisualDescriptor
    }
    class Station {
        -id_ : string
        -name_ : string
        -platforms_ : int
        +summary() string
        +toVisualDescriptor() VisualDescriptor
    }
    class Signal {
        -id_ : string
        -state_ : SignalState
        +summary() string
        +toVisualDescriptor() VisualDescriptor
    }
    class DispatcherModel {
        -elements_ : vector~unique_ptr~ISystemElement~~
        +addElement(unique_ptr) void
        +printSummary() void
        +collectVisualDescriptors() vector~VisualDescriptor~
    }

    ISystemElement <|.. Train : реализует
    ISystemElement <|.. Station : реализует
    ISystemElement <|.. Signal : реализует
    IVisualDescriptorProvider <|.. Train
    IVisualDescriptorProvider <|.. Station
    IVisualDescriptorProvider <|.. Signal
    DispatcherModel o-- ISystemElement : владеет unique_ptr
    DispatcherModel ..> IVisualDescriptorProvider : dynamic_cast
```

---

## Изменения в ЛР 9

- оформлена история Git;
- создана рабочая ветка;
- выполнено слияние;
- разобран простой конфликт;
- уточнён демонстрационный сценарий.
