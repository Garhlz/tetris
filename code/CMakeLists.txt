# CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(tetris)

# 设置C++标准为11 (可以考虑升级到 C++17 或更高，nlohmann-json 可能需要)
# 建议至少 C++11，C++17 更好
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------
# 修改部分：使用 vcpkg 管理依赖
# --------------------------------------------------------------------------
# vcpkg 会自动处理库的查找路径，因此 find_package 会直接工作
# ./vcpkg install glfw3 glm nlohmann-json opengl 
# opengl 通常不需要 vcpkg 安装，find_package(OpenGL) 会查找系统安装的

find_package(glfw3 CONFIG REQUIRED)      # 使用 CONFIG 模式查找 vcpkg 提供的配置文件
find_package(glm CONFIG REQUIRED)        # 同上
find_package(nlohmann_json CONFIG REQUIRED) # 查找 nlohmann-json
find_package(OpenGL REQUIRED)            # OpenGL 通常仍然由系统提供

# --------------------------------------------------------------------------
# 用add_executable定义名为目标 
# --------------------------------------------------------------------------
add_executable(tetris             
    main.cpp
    InitShader.cpp
    glad.c                  
    loadAndSave.h
    loadAndSave.cpp   
)

# --------------------------------------------------------------------------
# 指定包含目录
target_include_directories(tetris PRIVATE include) 

# --------------------------------------------------------------------------
# 为它链接所需的库 (修改链接目标)
# --------------------------------------------------------------------------
target_link_libraries(tetris PRIVATE
    glfw                            # glfw3 的目标名通常是 glfw (CMake 目标)
    glm::glm                        # glm 通常提供 glm::glm 目标
    nlohmann_json::nlohmann_json    # nlohmann_json 提供 nlohmann_json::nlohmann_json 目标
    OpenGL::GL                      # OpenGL 通常提供 OpenGL::GL 目标
)

