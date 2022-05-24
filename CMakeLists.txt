cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)
set(CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_MAKEFILE OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(OpenCvTest CXX)


# There HAS to be a better way of doing this ...
set(OpenCV_DIR "/opt/dev/opencv/lib/cmake/opencv4")
set(Qt5_DIR "/opt/dev/qt5/lib/cmake/Qt5")
set(spdlog_DIR "/opt/dev/spdlog/lib/cmake/spdlog")

#
# This is why we love Qt
# https://doc.qt.io/qt-5/cmake-get-started.html#

find_package(Qt5 COMPONENTS Core Widgets OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(spdlog REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

#include_directories(${OpenCV_INCLUDE_DIRS})
#include_directories(${Qt5_INCLUDE_DIRS})

file(GLOB SourceFiles ./src/*.cpp ./src/*.h)

add_executable(${CMAKE_PROJECT_NAME} ${SourceFiles})

target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} Qt5::Widgets spdlog::spdlog)
