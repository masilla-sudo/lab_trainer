# dispatcher_trainer_lab1
Лабораторная работа 1: настройка рабочей конфигурации C++17/20.
## Среда
Основной вариант: VS Code + Docker Dev Container.
Альтернативный вариант: Linux в VirtualBox.
## Как открыть проект
1. Установить VS Code, Docker и расширение Dev Containers.
2. Открыть каталог проекта в VS Code.
3. Выполнить команду: Dev Containers: Reopen in Container.
## Сборка вручную
```bash
cmake -S . -B build/debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug
./build/debug/dispatcher_trainer_lab1
```
## Сборка в VS Code
1. CMake: Configure
2. CMake: Build
3. CMake: Run Without Debugging
4. Run and Debug → Debug CMake target with CodeLLDB
## Проверено
- [ ] контейнер открывается;
- [ ] CMake configure проходит без ошибок;
- [ ] проект собирается;
- [ ] программа запускается;
- [ ] breakpoint срабатывает;
- [ ] сделан первый commit.
# dispatcher_trainer — Лабораторная работа 2

Консольный прототип диспетчерского тренажёра транспортного узла.

## Вариант

**B — Диспетчер транспортного узла.**

Параметры состояния:
- Загруженность маршрута, %
- Связь с объектом (да/нет)
- Число активных нарушений

## Сборка и запуск

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/dispatcher_trainer
## Будущая визуализация клиента
Вариант: диспетчер энергосети.
Планируемые элементы визуального ряда:
1. Карта объектов с маркерами подстанций.
2. Цветовой статус объекта: зелёный / жёлтый / красный.
3. Панель текущих параметров: нагрузка, связь, тревоги.4. AR-метка критического объекта над 3D-моделью.
5. AR-подсказка рекомендуемой команды диспетчера.