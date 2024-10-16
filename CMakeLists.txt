# 设置 cmake 的最低版本和项目名称
cmake_minimum_required(VERSION 3.5)  # 修改为 3.5 或更高
project(mprpc)

# set(CMAKE_BUILD_TYPE "DEBUG") #調試

# 设置可执行文件输出的路径为项目源代码目录下的 bin 目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置库文件输出的路径为项目源代码目录下的 lib 目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置编译时头文件的搜索路径，指定 src 和 example 目录
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)

# 设置库文件的搜索路径，指定 lib 目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 添加 src 子目录，CMake 将会寻找该目录下的 CMakeLists.txt 文件并执行
add_subdirectory(src)

# 添加 example 子目录，CMake 将会寻找该目录下的 CMakeLists.txt 文件并执行
add_subdirectory(example)
