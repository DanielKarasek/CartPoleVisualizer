cmake_minimum_required(VERSION 3.21)
project(CartPoleVisualizer)

set(CMAKE_PREFIX_PATH "/home/pako/Qt/6.2.2/gcc_64")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(sou)
add_compile_options(-Wall -Wextra -Wpedantic -Wno-psabi)

add_executable(CartPoleVisualizer
        src/main.cpp
        src/auxilaries.cpp
        src/cartpolerenderer.cpp
        src/cartpoletask.cpp
        src/humansolver.cpp
        src/mainwindow.cpp
        src/mediator.cpp
        src/nnsolver.cpp
        src/settingbar.cpp
        src/test.cpp
        src/mainwindow.ui
        include/CartPoleVisualizer/auxilaries.h
        include/CartPoleVisualizer/cartpolerenderer.h
        include/CartPoleVisualizer/cartpoletask.h
        include/CartPoleVisualizer/humansolver.h
        include/CartPoleVisualizer/mainwindow.h
        include/CartPoleVisualizer/mediator.h
        include/CartPoleVisualizer/nnsolver.h
        include/CartPoleVisualizer/settingbar.h)

target_include_directories(CartPoleVisualizer PRIVATE include/CartPoleVisualizer)

find_library(CustomQtWidgets lib/CustomQtWidgets)
find_library(neat lib/neat)
find_library(Environments lib/Environments)

add_subdirectory(lib/CustomQtWidgets)
add_subdirectory(lib/neat)
add_subdirectory(lib/Environments)

target_link_libraries(CartPoleVisualizer
        Qt::Core
        Qt::Gui
        Qt::Widgets
        CustomQtWidgets
        neat
        Environments
        )

