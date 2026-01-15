cmake_minimum_required(VERSION 3.16)
project(SnakeGame VERSION 1.0.0 LANGUAGES CXX)

# ==================== 编译配置 ====================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Windows 下隐藏控制台窗口
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# ==================== 查找 Qt ====================
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 COMPONENTS Core Gui Widgets REQUIRED)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

# ==================== 源文件 ====================

# 核心逻辑（后端）
set(CORE_SOURCES
    src/core/Snake.cpp
    src/core/Food.cpp
    src/core/GameLogic.cpp
)

set(CORE_HEADERS
    src/Constants/Constants.h
    src/Constants/Direction.h
    src/Constants/GameState.h
    src/Constants/RendererType.h
    src/core/Snake.h
    src/core/Food.h
    src/core/GameLogic.h
)

# UI（前端）
set(UI_SOURCES
    src/ui/MainWindow.cpp
    src/ui/GameWidget.cpp
    src/ui/SceneGameView.cpp
)

set(UI_HEADERS
    src/ui/MainWindow.h
    src/ui/GameWidget.h
    src/ui/SceneGameView.h
)

# 主程序
set(MAIN_SOURCES
    src/main.cpp
)

# ==================== 构建目标 ====================

# 核心逻辑库（可独立测试）
add_library(SnakeCore STATIC
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

target_include_directories(SnakeCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Constants
)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(SnakeCore PUBLIC Qt6::Core Qt6::Gui)
else()
    target_link_libraries(SnakeCore PUBLIC Qt5::Core Qt5::Gui)
endif()

# UI 库
add_library(SnakeUI STATIC
    ${UI_SOURCES}
    ${UI_HEADERS}
)

target_include_directories(SnakeUI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(SnakeUI PUBLIC SnakeCore Qt6::Widgets)
else()
    target_link_libraries(SnakeUI PUBLIC SnakeCore Qt5::Widgets)
endif()

# 主程序
add_executable(SnakeGame
    ${MAIN_SOURCES}
)

target_include_directories(SnakeGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

target_link_libraries(SnakeGame PRIVATE
    SnakeUI
)

# ==================== 输出信息 ====================
message(STATUS "Qt version: ${QT_VERSION_MAJOR}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
