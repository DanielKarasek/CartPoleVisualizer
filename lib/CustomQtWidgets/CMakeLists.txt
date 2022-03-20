cmake_minimum_required(VERSION 3.2)
project(CustomQtWidgets VERSION 0.1 LANGUAGES CXX)

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

set(SOURCES src/doublehintslider.cpp
            src/labeleddoubleslider.cpp)

set(HEADERS include/CustomQtWidgets/doublehintslider.h
            include/CustomQtWidgets/labeleddoubleslider.h)

add_library(CustomQtWidgets ${SOURCES} ${HEADERS})
add_compile_options(-Wall -Wextra -Werror -Wpedantic)

target_include_directories(CustomQtWidgets PUBLIC src include)
export(TARGETS CustomQtWidgets FILE CustomQtWidgets.cmake)
target_link_libraries(CustomQtWidgets
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )