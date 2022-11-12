# 查找当前目录下的所有源文件

# 并将名称保存到 DIR_ARMOR_TRACKER 变量

find_package(OpenCV  REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

aux_source_directory(. DIR_PARAMETER)

include_directories(include)

# 生成链接库

add_library (Parameter ${DIR_PARAMETER})

target_link_libraries(Parameter ${OpenCV_LIBS})

