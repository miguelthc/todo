cmake_minimum_required(VERSION 3.10)
project(todo)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-g)
endif()

add_executable(todo main.cpp cmd/cmd.cpp options/options.cpp)