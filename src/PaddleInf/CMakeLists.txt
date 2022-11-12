cmake_minimum_required(VERSION 3.5)

project(ORTI VERSION 0.1.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")
add_definitions(-std=c++11)
aux_source_directory(. DIR_PADDLEINF)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
link_directories(paddle_inference/paddle/lib)

find_package(OpenCV REQUIRED)

add_library(PaddleInf PaddleInf.cpp)

target_include_directories(PaddleInf PUBLIC include)

target_link_libraries( PaddleInf ${OpenCV_LIBS} paddle_inference)

