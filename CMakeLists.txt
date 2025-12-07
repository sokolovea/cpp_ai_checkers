cmake_minimum_required(VERSION 3.30)
project(checkers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включить все санитайзеры
set(SANITIZERS "address;undefined;leak")

foreach(SANITIZER ${SANITIZERS})
    add_compile_options(-fsanitize=${SANITIZER})
    add_link_options(-fsanitize=${SANITIZER})
endforeach()

# Добавление поддиректории с движком игры
add_subdirectory(logic)

add_executable(checkers main.cpp)
target_link_libraries(checkers PRIVATE logic)

target_compile_features(checkers PUBLIC cxx_std_20)

# Дополнительные флаги для отладочной сборки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(checkers PRIVATE
        -Wall
        -Wextra
        -O0
        -g
    )
endif()
