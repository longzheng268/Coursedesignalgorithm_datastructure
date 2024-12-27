cmake_minimum_required(VERSION 3.10)
project(algorithm_course_design C)

# 获取真实路径
get_filename_component(REAL_PATH "${CMAKE_SOURCE_DIR}" REALPATH)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置输出目录为真实路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${REAL_PATH}/build")
set(EXECUTABLE_OUTPUT_PATH "${REAL_PATH}/build")

# 查找GTK3包
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# 包含GTK3头文件
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

# 添加源文件
file(GLOB_RECURSE SOURCES 
    "src/*.c"
    "src/*/*.c"
)

# 创建资源同步目标
add_custom_target(sync_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${REAL_PATH}/resources
    ${CMAKE_BINARY_DIR}/resources
    COMMENT "Syncing resource files..."
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 添加依赖
add_dependencies(${PROJECT_NAME} sync_resources)

# 链接GTK3库
target_link_libraries(${PROJECT_NAME} 
    ${GTK3_LIBRARIES}
    m
    pthread
)

# 设置编译选项
target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall 
    -Wextra 
    -g
)

# 添加编译定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    MAX_SET_SIZE=1000
)

# 设置输出目录和可执行文件名称
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_NAME "algorithm_course_design"
)

# 设置资源文件目录
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
if(NOT EXISTS ${RESOURCE_DIR})
    file(MAKE_DIRECTORY ${RESOURCE_DIR})
endif()

# 添加背景图片目录
set(BACKGROUNDS_DIR "${RESOURCE_DIR}/backgrounds")
if(NOT EXISTS ${BACKGROUNDS_DIR})
    file(MAKE_DIRECTORY ${BACKGROUNDS_DIR})
endif()

# 添加测试文件目录
set(TEST_FILES_DIR "${RESOURCE_DIR}/test_files")
if(NOT EXISTS ${TEST_FILES_DIR})
    file(MAKE_DIRECTORY ${TEST_FILES_DIR})
endif()

# 安装目标
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# 安装资源文件
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/
    DESTINATION share/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*"
)