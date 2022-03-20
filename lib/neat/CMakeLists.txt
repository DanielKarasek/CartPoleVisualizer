cmake_minimum_required(VERSION 3.2)
project(neat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/home/pako/Qt/6.2.2/gcc_64")

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Werror -Wpedantic)

set(SOURCES
        src/activationfunctions.cpp
        src/auxilaries.cpp
        src/task.cpp
        src/neuralnet.cpp
        src/neat.cpp
        src/species.cpp
        src/genome.cpp)

set(HEADERS
        include/neat/activationfunctions.h
        include/neat/auxilaries.h
        include/neat/task.h
        include/neat/neuralnet.h
        include/neat/neat.h
        include/neat/species.h
        include/neat/genome.h)

add_library(neat ${SOURCES} ${HEADERS})

target_include_directories(neat PUBLIC src include)
target_include_directories(neat PRIVATE src include/neat)

export(TARGETS neat FILE neat.cmake)
