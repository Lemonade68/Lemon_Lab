cmake_minimum_required(VERSION 3.15)

#设置变量：set(Value_name Value)   注意都是字符串
#使用变量：${Value_name} 进行使用
set(CMAKE_CXX_STANDARD 17)  #设置c++版本
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Lemon_Lab LANGUAGES CXX)

# if(MSVC)
#   add_compile_options("/MP")
#   add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
#   add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
#   add_definitions(-D_USE_MATH_DEFINES)
# endif(MSVC)

# if(WIN32)
#   add_definitions(-DUNICODE -D_UNICODE)
#   add_definitions(-DNOMINMAX)
# endif()

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/lib)    # 设置默认存放静态库的位置
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/lib)    # 设置默认存放动态库的位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/bin)    # 可执行文件位置

# add_subdirectory(./src/CoreLayer/Math)
# add_subdirectory(./externals/glm)

# 第一个参数是exe的名字，后面为参与编译的文件(中间使用空格或者;分隔)
add_executable(Lemon_Lab 
 src/main.cpp)

target_include_directories(Lemon_Lab PUBLIC ./externals)

# 链接上glm库
# find_package(glm REQUIRED)
# target_link_libraries(Lemon_Lab glm::glm_shared)

# target_include_directories(Lemon_Lab PUBLIC ./src)
# target_include_directories(Lemon_Lab PUBLIC ./externals)

# target_link_directories(Lemon_Lab PUBLIC ${LIBS_PATH})
# target_link_directories(Lemon_Lab PUBLIC ${PROJECT_SOURCE_DIR}/target/lib)
