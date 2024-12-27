#!/bin/bash

# 获取本机IP地址
LOCAL_IP=$(hostname -I | cut -d' ' -f1)

# 设置DISPLAY环境变量
export DISPLAY=${LOCAL_IP}:0.0

# 运行程序
./algorithm_course_design 