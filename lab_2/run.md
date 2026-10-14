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
lab_2/
├── CMakeLists.txt        — сборка: библиотека polynomial, main, тесты
├── src/
│   ├── polynomial.h       — объявления класса, полей и методов
│   ├── polynomial.cpp     — реализация класса
│   └── main.cpp           — пример работы
├── tests/
│   └── test_polynomial.cpp  — 27 тестов Google Test
├── docs/
│   ├── lab_02_string_2026-08-19.pdf   — задание
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