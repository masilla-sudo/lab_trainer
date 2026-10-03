# ЛР 13: отладка и санитайзеры

## 1. Подготовка

Сборка с санитайзерами: CMake-опция `ENABLE_SANITIZERS`, в CMakeLists.txt
функция `enable_project_sanitizers` добавляет для всех целей
`-fsanitize=address,undefined -fno-omit-frame-pointer -g`.

Внесён временный дефект в `src/core/log.cpp` (printEventLog):
условие цикла `i <= log.size()` вместо `i < log.size()` —
на итерации i == size() читается элемент с индексом за границей
выделенного блока (см. раздел 2).

## 2. Воспроизведение дефекта

Условия: журнал заполнен до size == capacity == 16 событий
(9 стартовых + 7 добавленных вручную командой `event Info test`),
затем команда `log`.

Наблюдение (sanitizer-сборка, build-sanitize): запуск через
`setarch $(uname -m) -R ...` (см. примечание про ASLR ниже).

Полный отчёт ASan:

```text
==6216==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x61b000000700 at pc 0x555555601f80 bp 0x7fffffffccc0 sp 0x7fffffffccb8
READ of size 8 at 0x61b000000700 thread T0
    #0 0x555555601f7f in printEventLog(std::vector<EventRecord, std::allocator<EventRecord> > const&) /workspaces/dispatcher_trainer/src/core/log.cpp:27
    #1 0x5555555d986a in main /workspaces/dispatcher_trainer/src/app/main.cpp:81
    #2 0x7ffff6e12249 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #3 0x7ffff6e12304 in __libc_start_main_impl ../csu/libc-start.c:360
    #4 0x5555555d56c0 in _start (/workspaces/dispatcher_trainer/build-sanitize/dispatcher_trainer+0x816c0)

0x61b000000700 is located 0 bytes to the right of 1664-byte region [0x61b000000080,0x61b000000700)
allocated by thread T0 here:
    #0 0x7ffff79d34c8 in operator new(unsigned long) ../../../../src/libsanitizer/asan_new_delete.cpp:95
    #1 0x5555556163ca in std::__new_allocator<EventRecord>::allocate(unsigned long, void const*) /usr/include/c++/12/bits/new_allocator.h:137
    #2 0x555555614fd1 in std::allocator<EventRecord>::allocate(unsigned long) /usr/include/c++/12/bits/allocator.h:188
    #3 0x555555614fd1 in std::allocator_traits<std::allocator<EventRecord> >::allocate(std::allocator<EventRecord>&, void const*) /usr/include/c++/12/bits/alloc_traits.h:464
    #4 0x5555556115bb in std::_Vector_base<EventRecord, std::allocator<EventRecord> >::_M_allocate(unsigned long) /usr/include/c++/12/bits/stl_vector.h:378
    #5 0x55555560e2df in void std::vector<EventRecord, std::allocator<EventRecord> >::_M_realloc_insert<EventRecord>(...) /usr/include/c++/12/bits/vector.tcc:453
    #6 0x55555560c81a in EventRecord& std::vector<EventRecord, std::allocator<EventRecord> >::emplace_back<EventRecord>(EventRecord&&) /usr/include/c++/12/bits/vector.tcc:123
    #7 0x55555560a509 in std::vector<EventRecord, std::allocator<EventRecord> >::push_back(EventRecord&&) /usr/include/c++/12/bits/stl_vector.h:1294
    #8 0x555555601b02 in addEvent(...) /workspaces/dispatcher_trainer/src/core/log.cpp:17
    #9 0x5555555f12bc in applyCommand(...) /workspaces/dispatcher_trainer/src/core/model.cpp:135
    #10 0x5555555d86c2 in main /workspaces/dispatcher_trainer/src/app/main.cpp:44
    #11 0x7ffff6e12249 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58

SUMMARY: AddressSanitizer: heap-buffer-overflow /workspaces/dispatcher_trainer/src/core/log.cpp:27 in printEventLog(...)
Shadow bytes around the buggy address:
  0x0c367fff8090: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c367fff80a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c367fff80b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c367fff80c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c367fff80d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c367fff80e0: [fa] fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c367fff80f0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c367fff8100: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c367fff8110: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c367fff8120: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c367fff8130: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:     fa
  Freed heap region:     fd
  Stack left redzone:    f1
  Stack mid redzone:     f2
  Stack right redzone:   f3
  Stack after return:    f5
  Stack use after scope: f8
  Global redzone:        f9
  Global init order:     f6
  Poisoned by user:      f7
  Container overflow:    fc
  Array cookie:          ac
  Intra object redzone:  bb
  ASan internal:         fe
  Left alloca redzone:   ca
  Right alloca redzone:  cb
==6216==ABORTING
```

Примечание: PID в отчёте (6216) соответствует запуску под `script`; адреса могут
отличаться от запуска к запуску — это норма для ASLR, важна структура отчёта.

Ключевые факты из отчёта:
- тип дефекта: heap-buffer-overflow, чтение (READ of size 8);
- место: `src/core/log.cpp:27` — `const EventRecord& e = log[i];`;
- адрес ровно на границе блока: "0 bytes to the right of 1664-byte region";
- блок 1664 байта = 16 элементов EventRecord, выделен push_back в addEvent.

## 3. Исправление

src/core/log.cpp: условие цикла `i <= log.size()` заменено на
`i < log.size()`.

Регрессионный тест в tests/test_core.cpp
(test_print_event_log_at_full_capacity): журнал резервируется до 100
элементов (reserve), заполняется ровно до size == capacity — на этой
границе дефектный цикл выходил за блок памяти. До исправления вызов
printEventLog под ASan абортировался (heap-buffer-overflow на
log.cpp:27), после исправления тест проходит: выводятся записи [0]..[99]
и «All tests passed».

Проверка чувствительности регрессионного теста: дефект возвращён
временно (sed-подстановка `i <` → `i <=`), пересборка и запуск test_core
под ASan дали отчёт heap-buffer-overflow в printEventLog (log.cpp:25
в текущей редакции файла) — чтение на границе блока 10400 байт,
выделенного reserve(100) в test_core.cpp:218 (100 × 104 байта на
элемент), код возврата 1. Затем исправление возвращено — тесты снова
зелёные (см. раздел 4). Таким образом, регрессионный тест действительно
ловит именно этот дефект: с багом падает (красный прогон),
без — проходит (зелёный).

## 4. Проверка после исправления

Сборка build-sanitize: cmake --build build-sanitize — без ошибок.

ctest:
    1/2 Test #1: template_rules ........... Passed 0.02 sec
    2/2 Test #2: core ..................... Passed 0.02 sec
    100% tests passed, 0 tests failed out of 2

Регрессионный тест test_print_event_log_at_full_capacity:
заполняет журнал до size == capacity (reserve(100), 100 событий)
и вызывает printEventLog. Вывод: «Журнал событий (100):» с записями
[0]..[99] без элемента за границей, «All tests passed».

Контрольный запуск под ASan: 16 событий (size == capacity == 16),
команда log печатает ровно 16 строк [0]..[15], мусорной записи [16] нет,
отчётов AddressSanitizer нет, код возврата 0.

Сборка без санитайзеров (дефолтная, ENABLE_SANITIZERS=OFF):
cmake -S . -B build && cmake --build build -j — без ошибок;
ctest --test-dir build — 2/2 Passed. Исправление работает независимо
от ASan, опция санитайзеров обычный путь сборки не сломала.

## Примечание про ASLR

ASan-рантайм GCC 12 падает с бесконечным DEADLYSIGNAL при повышенной
энтропии ASLR ядра (vm.mmap_rnd_bits = 32). Обход внутри контейнера —
запуск бинарника с отключённой для процесса рандомизацией:
`setarch $(uname -m) -R ./build-sanitize/dispatcher_trainer`.
Проверено: прямой запуск test_core/test_template_rules без setarch
воспроизводит бесконечный DEADLYSIGNAL, запуск через setarch -R —
чистый (ctest 2/2 Passed, код возврата 0).
Постоянное исправление на хосте: `sudo sysctl -w vm.mmap_rnd_bits=28`.
