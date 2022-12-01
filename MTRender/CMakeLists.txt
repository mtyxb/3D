cmake_minimum_required(VERSION 3.5)

project(MTRender LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(MTRender
main.cpp
mtwidget.cpp
mtwidget.h
mtvector.cpp
mtvector.h
mtmatrix.cpp
mtmatrix.h
mtpipeline.cpp
mtpipeline.h
mttexture.cpp
mttexture.h
mtraster.cpp
mtraster.h
testcase.cpp
)

target_link_libraries(MTRender PRIVATE Qt5::Widgets)
