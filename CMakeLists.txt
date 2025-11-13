cmake_minimum_required(VERSION 3.30)
project(checkers)

set(CMAKE_CXX_STANDARD 20)

add_executable(checkers main.cpp
        Situation.cpp
        Situation.h
        Cell.h
        Cell.h
        Coordinate.h
        EnumGameCurrentStatus.h
        AiSolver.h
        AiSolver.h
        Tree.h
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -O0 -g3 -fsanitize=address,undefined,leak")
