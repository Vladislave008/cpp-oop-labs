cmake_minimum_required(VERSION 3.20)
project(lab3_variant19 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    set(LAB_WARNINGS /W4)
else()
    set(LAB_WARNINGS -Wall -Wextra -Wpedantic)
endif()

add_library(exam_grade_set STATIC src/exam_grade.cpp src/exam_grade_set.cpp)
target_include_directories(exam_grade_set PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(exam_grade_set PRIVATE ${LAB_WARNINGS})

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE exam_grade_set)
target_compile_options(main PRIVATE ${LAB_WARNINGS})

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
add_executable(test_exam_grade_set tests/test_exam_grade_set.cpp)
target_link_libraries(test_exam_grade_set PRIVATE exam_grade_set GTest::gtest_main)
target_compile_options(test_exam_grade_set PRIVATE ${LAB_WARNINGS})

include(GoogleTest)
gtest_discover_tests(test_exam_grade_set)
