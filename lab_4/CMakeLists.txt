cmake_minimum_required(VERSION 3.20)
project(lab_4_bank LANGUAGES CXX)

add_library(bank_products
    src/bank.cpp
    src/deposit.cpp
    src/credit.cpp
    src/card.cpp
    src/bank_collection.cpp
)
target_include_directories(bank_products PUBLIC src)
target_compile_features(bank_products PUBLIC cxx_std_20)
set_target_properties(bank_products PROPERTIES CXX_EXTENSIONS OFF)

add_executable(lab_4 src/main.cpp)
target_link_libraries(lab_4 PRIVATE bank_products)
set_target_properties(lab_4 PROPERTIES CXX_EXTENSIONS OFF)

foreach(target bank_products lab_4)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()

include(CTest)
if(BUILD_TESTING)
    include(FetchContent)
    if(POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
    endif()
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(bank_tests tests/test_bank.cpp)
    target_link_libraries(bank_tests PRIVATE bank_products GTest::gtest_main)
    set_target_properties(bank_tests PROPERTIES CXX_EXTENSIONS OFF)
    if(MSVC)
        target_compile_options(bank_tests PRIVATE /W4)
    else()
        target_compile_options(bank_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    include(GoogleTest)
    gtest_add_tests(TARGET bank_tests SOURCES tests/test_bank.cpp)
endif()
