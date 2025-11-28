cmake_minimum_required(VERSION 3.30)
project(checkers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление поддиректории с движком игры
add_subdirectory(logic)

add_executable(checkers main.cpp)
target_link_libraries(checkers PRIVATE logic)

target_compile_features(checkers PUBLIC cxx_std_20)

