# 兰州理工大学《算法与数据结构》课程设计

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)
[![Repo](https://img.shields.io/badge/GitHub-longzheng268%2FCoursedesignalgorithm_datastructure-black?logo=github)](https://github.com/longzheng268/Coursedesignalgorithm_datastructure)

本仓库用于兰州理工大学《算法与数据结构》课程设计的实现与总结。项目以 C 语言为主，提供一个基于 GTK3 的图形界面应用，集成了多类经典数据结构与算法的可视化/交互式演示与实验。

- 默认分支：main
- 主要技术：C11、GTK3、CMake、Make
- 许可协议：GPL-3.0

---

## 功能概览

- 图形化界面（GTK3）
  - Notebook 多页标签组织各算法模块
  - 背景图片自动轮播（支持自适应窗口缩放）
  - 全局字体缩放（内置样式与控件）
- 模块与专题
  - 集合运算：并集、交集、差集等（模块：src/union）
  - 递归与包含处理：对文本包含指令（include）进行递归展开与格式化（模块：src/recursion）
  - 哈夫曼（Huffman）编码与解码：构建树、统计频率、输出编码（模块：src/huffman）
  - 排序与序列构造：A 与 D 序列互构，排序演示（模块：src/sorting）
  - 统一错误处理与日志：弹窗提示与 error.log 记录（模块：src/utils）
- 工程化支持
  - CMake 构建（可跨平台）
  - 顶层 Makefile 一键调用 CMake
  - 资源目录自动同步到构建输出目录

---

## 截图

![image](https://github.com/user-attachments/assets/413515de-5f7a-4cce-9993-d2a51a2cddd2)
![image](https://github.com/user-attachments/assets/795d1976-8c5a-42d8-89d7-31ae6a357836)

---

## 目录结构

实际目录可能会随开发演进有所调整，以下为关键路径说明：

```
.
├─ src/                          # 源码（按专题划分）
│  ├─ main.c                     # 应用入口，初始化窗口/背景/字体缩放/分页
│  ├─ union/                     # 集合运算（并/交/差 等）
│  ├─ recursion/                 # 递归 include 处理与内容格式化
│  ├─ huffman/                   # 哈夫曼编码/解码与树构建、最小堆
│  ├─ sorting/                   # 排序与 A/D 序列构造（见 sorting.h）
│  └─ utils/                     # 错误处理与通用工具（GTK 弹窗 + 日志）
├─ resources/                    # 资源目录（构建时自动创建/同步）
│  ├─ backgrounds/               # 背景图片（可自行放置 jpg/png）
│  └─ test_files/                # 示例/测试文件
├─ CMakeLists.txt                # CMake 构建脚本（输出名：algorithm_course_design）
├─ Makefile                      # 一键驱动 CMake 的简易脚本
├─ LICENSE                       # GPL-3.0 许可
└─ README.md
```

提示：
- 构建过程中会将 resources/ 同步到构建输出目录（build/resources）。
- CMake 会确保 resources、backgrounds、test_files 目录存在。

---

## 环境依赖

- 操作系统：Linux / macOS / Windows（建议 Windows 使用 MSYS2 或 WSL2）
- 编译器：GCC 10+ 或 Clang 12+
- 构建工具：CMake 3.10+，Make 4.0+
- 图形库：GTK3（含开发头文件）与 pkg-config

安装参考（示例）：
- Debian/Ubuntu
  - sudo apt update
  - sudo apt install -y build-essential cmake pkg-config libgtk-3-dev
- Arch/Manjaro
  - sudo pacman -S --needed base-devel cmake pkgconf gtk3
- Fedora
  - sudo dnf install -y @development-tools cmake pkgconf-pkg-config gtk3-devel
- macOS（Homebrew）
  - brew install cmake gtk+3 pkg-config
- Windows（MSYS2）
  - 使用 MSYS2 MINGW64 环境：
  - pacman -S --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-gtk3 pkgconf

---

## 快速开始

1）克隆仓库
```bash
git clone https://github.com/longzheng268/Coursedesignalgorithm_datastructure.git
cd Coursedesignalgorithm_datastructure
```

2）准备资源（可选）
- 将你喜欢的背景图片放到 resources/backgrounds/ 以获得更好的视觉效果。

3）使用 CMake 构建（推荐）
```bash
# 在仓库根目录
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

4）或使用顶层 Make 构建
```bash
# 会在 ./build 目录内执行 CMake 并编译
make build
```

5）运行
```bash
# 可执行文件默认位于 ./build
./build/algorithm_course_design
```

---

## 使用说明（GUI）

- 启动后将看到包含多个页签的界面（如：集合、递归、哈夫曼、排序 等）。
- 背景图会定时更换；窗口缩放后背景会自适应。
- 右上或界面内提供字体缩放控件，可整体放大/缩小界面字体。

各模块要点：
- 集合运算（src/union）
  - 在两个输入框分别填入整数序列（空格或逗号分隔），点击并/交/差按钮得到结果。
- 递归包含（src/recursion）
  - 读取并处理文本内容中的“include”式指令，演示递归展开与格式化。
- 哈夫曼编码（src/huffman）
  - 输入文本后统计字符频率，生成哈夫曼树并显示编码；可对文本进行编码/解码演示。
- 排序与序列构造（src/sorting）
  - 提供 A -> D 与 D -> A 转换的方法与排序操作，便于观察复杂度与结果正确性。

错误处理与日志：
- 发生错误时会弹出 GTK 对话框提示，并写入项目根目录的 error.log 便于排查。

---

## 开发与贡献

- 代码风格
  - 建议使用 clang-format 统一代码风格（如仓库提供 .clang-format 则以其为准）。
  - 关键算法与数据结构请配备清晰注释与复杂度说明。
- 提交信息
  - 推荐遵循 Conventional Commits（如 feat/fix/docs/refactor/test/chore/build/perf/style）。
  - 示例：feat(huffman): add min-heap and code table management
- 调试与分析
  - 构建 Debug：cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
  - 运行时内存检查：valgrind --leak-check=full ./build/algorithm_course_design
  - 性能分析（gprof/其它工具）可按需开启对应编译选项。

---

## 常见问题（FAQ）

- 找不到 GTK3 头文件或库
  - 请确认已安装 GTK3 的“开发包”（例如 libgtk-3-dev / gtk3-devel / mingw-w64-x86_64-gtk3）。
  - 确认 pkg-config 可找到 gtk+-3.0：pkg-config --cflags gtk+-3.0
- 运行时出现找不到动态库（Windows）
  - 确认使用 MSYS2 MINGW64 终端运行，或将相应的 GTK3 动态库加入 PATH。
- 背景图片未显示或提示找不到
  - 确认 resources/backgrounds/ 下存在图片文件；构建时会自动同步到 build/resources。
- 中文字符显示异常
  - 确认系统字体与终端/桌面环境为 UTF-8；必要时安装中文字体包。

---

## 学术诚信

本项目旨在学习与交流。若用于课程作业提交，请严格遵守所在课程/学院的学术诚信要求，独立完成、禁止抄袭。引用本仓库内容时请注明来源。

---

## 许可协议

本项目基于 GNU GPL-3.0 许可协议开源，详见 [LICENSE](LICENSE)。

---

## 参考与致谢

- CLRS《算法导论》
- Sedgewick & Wayne《Algorithms, 4th Edition》
- 严蔚敏 等《数据结构》
- CP-Algorithms、VisuAlgo 等开源资料

---

## 维护者

- @longzheng268

欢迎提出 Issue 或提交 PR，一起完善本课程设计项目！
