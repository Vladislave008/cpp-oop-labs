# Files
cmake_minimum_required(VERSION 3.16)
project(lab2_variant14 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    set(LAB_WARNINGS /W4)
else()
    set(LAB_WARNINGS -Wall -Wextra -Wpedantic)
endif()

add_library(polynomial STATIC src/polynomial.cpp)
target_include_directories(polynomial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(polynomial PRIVATE ${LAB_WARNINGS})

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE polynomial)
target_compile_options(main PRIVATE ${LAB_WARNINGS})

# Tests
include(FetchContent)
set(CMP0135 NEW)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(test_polynomial tests/test_polynomial.cpp)
target_link_libraries(test_polynomial PRIVATE polynomial GTest::gtest_main)
target_compile_options(test_polynomial PRIVATE ${LAB_WARNINGS})

include(GoogleTest)
gtest_discover_tests(test_polynomial)
