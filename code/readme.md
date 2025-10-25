
实验环境：
OS: Arch Linux x86_64
Host: ThinkPad T480s
CPU: Intel(R) Core(TM) i5-8350U (8) @ 3.60 GHz 
GPU: Intel UHD Graphics 620 @ 1.10 GHz [Integrated]
包管理工具：pacman
依赖：OpenGL, GLFW, GLAD, GLM
gcc (GCC) 15.2.1 20250813
cmake 4.1.1-1 
glfw 1:3.4-1 
glm 1.0.1-1 
mesa 1:25.2.3-2

---

对于我的arch linux环境，需要修改的配置：

1. 从include中把两个文件夹复制过来

2. glad.c切换为自己的新版本的
   
3. linux环境：
```cpp
#define fopen_s(pFile, filename, mode) ((*(pFile)) = fopen((filename), (mode))) == NULL
```

1. CMakeLists.txt换为自己的版本。如果有官方的新文件，需要添加到add_executable中
   
2. main.c的main()中在init（）函数之后加入一段代码：
```cpp
int width, height;
glfwGetFramebufferSize(window, &width, &height);
glViewport(0, 0, width, height);	
```

---
## pacman 版本
编译运行方法：
- 构建：
```bash
mkdir build; cd build; cmake ..; make
```

- 运行
```bash
cd..; ./build/main
```

- 修改之后运行：
```bash
cd build;make;cd ..;./build/main
```

---

## vcpkg 版本
- 使用vcpkg进行构建：
```bash
mkdir build; cd build;cmake .. -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake;make
```

- 运行
```bash
cd ..; ./build/tetris
```

- 修改之后运行：
```bash
cd build;make;cd ..;./build/tetris
```