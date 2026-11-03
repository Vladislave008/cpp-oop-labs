# Сборка и запуск

Требуются CMake 3.20+ и компилятор с поддержкой C++20. Google Test 1.14.0 загружается через FetchContent.

Из каталога `lab_3` под Linux:

```sh
cmake -S . -B build
cmake --build build
./build/main
ctest --test-dir build --output-on-failure
```

Проверка с санитайзерами при наличии ASan/UBSan:

```sh
cmake -S . -B build-asan -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -g"
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure
```

Сборка отчёта через XeLaTeX, два прохода:

```sh
cd docs/report
xelatex report.tex
xelatex report.tex
```
