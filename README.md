# 算法与数据结构课程设计展示系统

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Platform](https://img.shields.io/badge/platform-Linux-blue.svg)]()
[![GTK](https://img.shields.io/badge/GTK-3.0-orange.svg)]()

> 兰州理工大学算法与数据结构课程设计 - 一个基于GTK3的图形化算法演示系统

## 📋 项目简介

本项目是兰州理工大学算法与数据结构课程的综合设计作业，使用C语言和GTK3图形库开发的桌面应用程序。系统集成了四个核心算法模块，提供直观的图形界面和交互式操作体验，帮助学生更好地理解和掌握算法与数据结构的核心概念。

### ✨ 主要特性

- 🎨 **现代化UI设计** - 基于GTK3的美观界面，支持动态背景和字体缩放
- 🧮 **集合运算模块** - 实现并集、交集、差集等基本集合操作
- 📝 **递归替换模块** - 智能处理C/C++源文件的include指令替换
- 🔤 **哈夫曼编码模块** - 完整的文本压缩编码和解码系统
- 🔄 **排序重构模块** - 数组排序和重构算法的可视化演示
- 🛡️ **错误处理系统** - 完善的异常处理和用户友好的错误提示
- 📱 **响应式布局** - 支持窗口大小调整和动态字体缩放

## 🚀 快速开始

### 系统要求

- **操作系统**: Linux (Ubuntu 18.04+ / CentOS 7+ / Debian 9+)
- **编译器**: GCC 7.0+ 或 Clang 6.0+
- **构建工具**: CMake 3.10+ 或 GNU Make
- **图形库**: GTK+ 3.24+
- **其他依赖**: glib2, gdk-pixbuf2

### 安装依赖

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake libgtk-3-dev pkg-config
```

#### CentOS/RHEL/Fedora
```bash
# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install cmake gtk3-devel pkgconf-pkg-config

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake gtk3-devel pkgconf-pkg-config
```

#### Arch Linux
```bash
sudo pacman -S base-devel cmake gtk3 pkgconf
```

### 编译安装

```bash
# 克隆项目
git clone https://github.com/longzheng268/Coursedesignalgorithm_datastructure.git
cd Coursedesignalgorithm_datastructure

# 编译项目
make build
# 或者使用CMake
# mkdir build && cd build
# cmake .. && make

# 运行程序
./build/algorithm_course_design
```

## 📚 功能模块

### 1. 集合运算 (Set Operations)

实现基本的集合运算功能，支持：

- **并集运算** - 计算两个集合的并集
- **交集运算** - 计算两个集合的交集  
- **差集运算** - 计算A-B和B-A的差集

**使用方法**:
1. 在"集合1"和"集合2"输入框中输入数字（用空格或逗号分隔）
2. 点击相应的运算按钮查看结果
3. 结果会自动去重并排序显示

**示例**:
```
集合1: 1 2 3 4 5
集合2: 3 4 5 6 7
并集结果: {1, 2, 3, 4, 5, 6, 7}
交集结果: {3, 4, 5}
差集(A-B)结果: {1, 2}
差集(B-A)结果: {6, 7}
```

### 2. 递归替换 (Recursive Include Replacement)

智能处理C/C++源文件中的#include指令，将包含文件的内容递归地替换到主文件中。

**特性**:
- 递归处理嵌套包含文件
- 保持代码格式和注释
- 智能路径解析
- 防止无限递归
- 中文字符格式化处理

**使用方法**:
1. 点击"选择文件"按钮选择C/C++源文件
2. 系统会自动处理所有#include指令
3. 在输出区域查看展开后的完整代码

### 3. 哈夫曼编码 (Huffman Coding)

完整的哈夫曼编码系统，实现文本的压缩编码和解码。

**核心算法**:
- 字符频率统计
- 最小堆构建
- 哈夫曼树生成
- 编码表构建
- 文本编码/解码

**使用方法**:
1. 在输入框中输入要编码的文本
2. 点击"编码"按钮生成哈夫曼树和编码表
3. 编码结果显示在输出区域
4. 可以对编码结果进行解码验证

**示例**:
```
输入文本: "hello world"
编码表:
l: 00
o: 01
e: 100
h: 101
[space]: 110
w: 1110
r: 11110
d: 11111
编码结果: 10110000001110100100000111011111011001111
```

### 4. 排序重构 (Sorting Reconstruction)

实现数组的排序和重构算法，包括：

**构造D操作**:
- 输入数列A（要求A[0]=0）
- 计算D[i,j] = A[i] - A[j]
- 输出所有可能的差值

**构造A操作**:
- 输入数列D
- 重构出可能的原数列A
- 验证重构结果的正确性

**使用方法**:
1. 输入数列（用空格或逗号分隔）
2. 选择"构造D"或"构造A"操作
3. 查看计算结果和验证信息

## 🎯 技术架构

### 项目结构
```
Coursedesignalgorithm_datastructure/
├── src/                    # 源代码目录
│   ├── main.c             # 主程序入口
│   ├── huffman/           # 哈夫曼编码模块
│   │   ├── huffman.c
│   │   └── huffman.h
│   ├── union/             # 集合运算模块
│   │   ├── union.c
│   │   └── union.h
│   ├── sorting/           # 排序重构模块
│   │   ├── sorting.c
│   │   └── sorting.h
│   ├── recursion/         # 递归替换模块
│   │   ├── recursion.c
│   │   └── recursion.h
│   └── utils/             # 工具函数
│       ├── error_handler.c
│       └── error_handler.h
├── resources/             # 资源文件
│   ├── backgrounds/       # 背景图片
│   └── test_files/       # 测试文件
├── build/                 # 构建输出目录
├── CMakeLists.txt        # CMake配置文件
├── Makefile              # Make配置文件
├── run.sh                # 运行脚本
└── README.md            # 项目说明文档
```

### 核心技术

- **编程语言**: C11标准
- **图形界面**: GTK+ 3.0
- **构建系统**: CMake 3.10+ / GNU Make
- **内存管理**: 手动内存管理和RAII模式
- **错误处理**: 基于异常上下文的错误处理机制
- **数据结构**: 堆、树、数组、链表等

### 设计模式

- **模块化设计**: 每个功能模块独立开发和维护
- **MVC架构**: 分离界面、逻辑和数据管理
- **工厂模式**: 统一的页面创建接口
- **观察者模式**: 事件驱动的用户交互

## 🛠️ 开发指南

### 编译选项

```bash
# Debug模式编译
mkdir build-debug && cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Release模式编译
mkdir build-release && cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# 添加编译参数
cmake -DCMAKE_C_FLAGS="-Wall -Wextra -O2" ..
```

### 测试运行

```bash
# 基本运行
./build/algorithm_course_design

# 带调试信息
G_DEBUG=all ./build/algorithm_course_design

# 内存检查（需要valgrind）
valgrind --leak-check=full ./build/algorithm_course_design
```

### 代码风格

项目遵循以下代码规范：
- 使用4空格缩进
- 函数名使用snake_case
- 常量使用UPPER_CASE
- 结构体使用PascalCase
- 详细的注释和文档

## 📸 应用截图

### 主界面
![主界面](https://github.com/user-attachments/assets/413515de-5f7a-4cce-9993-d2a51a2cddd2)

*程序主界面，展示了现代化的标签页设计和动态背景*

### 功能模块
![功能演示](https://github.com/user-attachments/assets/795d1976-8c5a-42d8-89d7-31ae6a357836)

*各功能模块的操作界面，包括集合运算、哈夫曼编码等*

## 🐛 问题排查

### 常见问题

**1. 编译错误：找不到GTK头文件**
```bash
# 解决方案：安装GTK开发包
sudo apt install libgtk-3-dev  # Ubuntu/Debian
sudo yum install gtk3-devel    # CentOS/RHEL
```

**2. 运行时错误：无法连接到显示服务器**
```bash
# 解决方案：设置DISPLAY环境变量
export DISPLAY=:0.0
# 或者使用SSH X11转发
ssh -X username@hostname
```

**3. 中文显示异常**
```bash
# 解决方案：设置正确的区域设置
export LANG=zh_CN.UTF-8
export LC_ALL=zh_CN.UTF-8
```

### 调试技巧

- 使用`G_DEBUG=all`启用GTK调试信息
- 使用`gdb`调试器排查崩溃问题
- 检查`error.log`文件获取详细错误信息
- 使用`strace`追踪系统调用

## 🤝 贡献指南

欢迎参与项目开发和改进！

### 如何贡献

1. Fork 本项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启一个 Pull Request

### 开发规范

- 遵循现有的代码风格
- 添加必要的注释和文档
- 编写单元测试（如适用）
- 更新README文档（如有新功能）

## 📄 许可证

本项目基于 MIT 许可证开源 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 👨‍💻 作者信息

- **作者**: 龙正 (LongZheng)
- **学号**: 210052202019
- **学校**: 兰州理工大学
- **专业**: 计算机科学与技术
- **邮箱**: longzheng268@gmail.com
- **GitHub**: [@longzheng268](https://github.com/longzheng268)

## 📞 联系方式

如有问题或建议，请通过以下方式联系：

- 📧 **邮箱**: longzheng268@gmail.com
- 🐙 **GitHub Issues**: [项目Issues页面](https://github.com/longzheng268/Coursedesignalgorithm_datastructure/issues)
- 💬 **QQ**: [在GitHub Profile查看]

## 🙏 致谢

- 感谢兰州理工大学计算机科学与技术专业的老师们的指导
- 感谢GTK+开发团队提供的优秀图形库
- 感谢开源社区提供的各种工具和资源

---

<div align="center">

**⭐ 如果这个项目对你有帮助，请给一个Star！⭐**

Made with ❤️ by [龙正](https://github.com/longzheng268)

</div>
