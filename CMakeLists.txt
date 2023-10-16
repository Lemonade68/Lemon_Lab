cmake_minimum_required(VERSION 3.15)

#设置变量：set(Value_name Value)   注意都是字符串
#使用变量：${Value_name} 进行使用
set(CMAKE_CXX_STANDARD 17)  #设置c++版本
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Lemon_Lab LANGUAGES CXX)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/lib)    # 设置默认存放静态库的位置
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/bin)    # 设置默认存放动态库的位置(和exe放在一起，这样exe运行时可以找到)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/bin)    # 可执行文件位置

# add_subdirectory(./src/CoreLayer/Math)
# add_subdirectory(./externals/glm)
# add_subdirectory(./src/CoreLayer/Math)    #这个加了会出错  不知道为啥

# file(GLOB_RECURSE ALL_INCLUDE CONFIGURE_DEPENDS "*.h")
# file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS "*.cpp")

# add_executable(Lemon_Lab ${ALL_INCLUDE} ${ALL_SOURCES})

# 第一个参数是exe的名字，后面为参与编译的文件(中间使用空格或者;分隔)
add_executable(Lemon_Lab 
    src/main.cpp
    src/CoreLayer/Math/Transform.cpp
    src/FunctionLayer/Shape/Shape.cpp
    src/FunctionLayer/Shape/Sphere.cpp
    src/FunctionLayer/Sampler/IndependentSampler.cpp
    src/FunctionLayer/Camera/Camera.cpp
    src/FunctionLayer/Camera/Pinhole.cpp
    src/FunctionLayer/Integrator/NormalIntegrator.cpp
    src/FunctionLayer/Shape/Parallelogram.cpp
    src/FunctionLayer/Scene/Scene.cpp
    src/FunctionLayer/Light/AreaLight.cpp
    src/FunctionLayer/Material/Material.cpp
    src/FunctionLayer/Material/Matte.cpp
    src/FunctionLayer/Integrator/DirectIntegrator.cpp
    src/FunctionLayer/Material/Mirror.cpp)

target_include_directories(Lemon_Lab PUBLIC 
                                ${PROJECT_SOURCE_DIR}/externals  
                                ${PROJECT_SOURCE_DIR}/src)

# 链接上glm库(将其变成库形式链接)
# find_package(glm REQUIRED)
# target_link_libraries(Lemon_Lab glm::glm_shared)

# target_include_directories(Lemon_Lab PUBLIC ./src)
# target_include_directories(Lemon_Lab PUBLIC ./externals)

# target_link_directories(Lemon_Lab PUBLIC ${LIBS_PATH})
# target_link_directories(Lemon_Lab PUBLIC ${PROJECT_SOURCE_DIR}/target/lib)
