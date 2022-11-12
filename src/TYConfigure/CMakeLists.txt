# 查找当前目录下的所有源文件

# 并将名称保存到 DIR_LIB_SRCS 变量

#aux_source_directory(. DIR_TYCONFIGURE)
#
#find_package(OpenCV REQUIRED)
#
#include_directories(${OpenCV_INCLUDE_DIRS})
#
#add_definitions(-std=c++14 -O3)
#
#set(ABSOLUTE_TARGET_LIB tycam)
#add_library(${ABSOLUTE_TARGET_LIB} SHARED IMPORTED)
#set(ABSOLUTE_TARGET_LIB -ltycam)
## 生成链接库
#
#add_library (TYConfigure ${DIR_TYCONFIGURE})
#
#target_include_directories(TYConfigure PUBLIC include)
#target_link_libraries(TYConfigure ${OpenCV_LIBS})
#target_link_libraries(TYConfigure ${ABSOLUTE_TARGET_LIB})

