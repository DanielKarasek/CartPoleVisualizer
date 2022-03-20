cmake_minimum_required(VERSION 3.2)
project(Environments VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/home/pako/Qt/6.2.2/gcc_64")

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Werror -Wpedantic)

set(SOURCES
        src/CartPole/CartPole.cpp
        src/auxilaries.cpp)

set(HEADERS
        include/Environments/CartPole/CartPole.h
        include/Environments/auxilaries.h)

add_library(Environments ${SOURCES} ${HEADERS})

target_include_directories(Environments PUBLIC src include)
target_include_directories(Environments PRIVATE src include/Environments)

export(TARGETS Environments FILE Environments.cmake)
