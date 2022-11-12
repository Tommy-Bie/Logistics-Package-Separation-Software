cmake_minimum_required(VERSION 3.5)
project(PackSeg LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
link_directories(PaddleInf/paddle_inference/paddle/lib)
include_directories(Parameter/include)

aux_source_directory(. DIR_SRCS)
add_subdirectory(PackDetect)
#add_subdirectory(TYConfigure)
add_subdirectory(Parameter)
add_subdirectory(ParallelProcess)
add_subdirectory(Tracker)
add_subdirectory(PaddleInf)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

add_executable(PackSeg ${PROJECT_SOURCES})
target_link_libraries(PackSeg PackSegmentation)
#target_link_libraries(PackSeg TYConfigure)
target_link_libraries(PackSeg Parameter)
target_link_libraries(PackSeg ParallelProcess)
target_link_libraries(PackSeg PaddleInf)
target_link_libraries(PackSeg Tracker)
target_link_libraries(PackSeg ${OpenCV_LIBS})
target_link_libraries(PackSeg Qt${QT_VERSION_MAJOR}::Widgets)
set_property(TARGET PackSeg PROPERTY CXX_STANDARD 17)
set (EXECUTABLE_OUTPUT_PATH ../../Loadings)

