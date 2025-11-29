cmake_minimum_required(VERSION 3.30)
project(logic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    situation.cpp
)

set(HEADERS
    situation.h
    cell.h
    coordinate.h
    enum_game_current_status.h
    ai_solver.h
    tree.h
)

add_library(logic ${SOURCES})

target_include_directories(logic
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(logic PUBLIC cxx_std_20)