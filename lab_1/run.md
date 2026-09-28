# Сборка и запуск

## Требования

| Компонент | Версия |
|---|---|
| CMake | 3.16+ |
| Стандарт C++ | C++20 |
| Компилятор | GCC, Clang или MSVC |
| Тесты | Google Test 1.14.0 (подтягивается автоматически через `FetchContent`) |


## Команды (под Linux)

Все команды выполняются из корня лабораторной работы.

```sh
# Конфигурация и сборка
cmake -S . -B build
cmake --build build

# Запуск программы
./build/main

# Тесты
ctest --test-dir build --output-on-failure
```

## Структура проекта

```
lab_1/
├── CMakeLists.txt        — сборка: библиотека array_ops, main, тесты
├── src/
│   ├── array_ops.h       — объявления чистых функций
│   ├── array_ops.cpp     — реализация операций над динамическим массивом
│   └── main.cpp          — текстовое меню, ввод-вывод, обработка ошибок
├── tests/
│   └── test_variant.cpp  — 26 тестов Google Test
├── docs/
│   ├── lab_01_basics_2026-08-16.pdf   — задание
│   └── report/                        — отчёт по лабораторной работе
└── build/                — каталог сборки, создаётся автоматически
```

## Отчёт

Отчёт лежит в `docs/report` и собирается **XeLaTeX** (два прохода, для
оглавления):

```sh
cd docs/report
xelatex report.tex
xelatex report.tex
```