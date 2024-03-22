cmake_minimum_required(VERSION 3.24.0)
project(Qt_Test)

# Blog: https://blog.csdn.net/fengbingchun/article/details/135887666

message(STATUS "build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.8.0/msvc2019_64/lib/cmake/Qt6") # Qt6Config.cmake file location

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)
qt_standard_project_setup()

# Widgets_Test
set(SRCS_WIDGETS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../demo/Widgets_Test)
set(SRCS_WIDGETS
    ${SRCS_WIDGETS_PATH}/main.cpp
    ${SRCS_WIDGETS_PATH}/Widgets_Test.cpp
    ${SRCS_WIDGETS_PATH}/Widgets_Test.ui
)

add_executable(Widgets_Test ${SRCS_WIDGETS})
target_include_directories(Widgets_Test PRIVATE ${SRCS_WIDGETS_PATH})
target_link_libraries(Widgets_Test PRIVATE Qt6::Widgets)
# set_target_properties(Widgets_Test PROPERTIES # no need
#     WIN32_EXECUTABLE ON
#     MACOSX_BUNDLE ON
# )

# Console_Test
set(SRCS_CONSOLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../demo/Console_Test)
aux_source_directory(${SRCS_CONSOLE_PATH} SRCS_CONSOLE)

add_executable(Console_Test ${SRCS_CONSOLE})
target_include_directories(Console_Test PRIVATE ${SRCS_CONSOLE_PATH})
target_link_libraries(Console_Test PRIVATE Qt6::Core)

# Quick_Test
#set(CMAKE_AUTOMOC ON) # no need

set(SRCS_QUICK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../demo/Quick_Test)
set(SRCS_QUICK
    ${SRCS_QUICK_PATH}/main.cpp
)

add_definitions(-DCMAKE_BUILD_METHOD)

add_executable(Quick_Test ${SRCS_QUICK}) # you can also use qt_add_executable
target_link_libraries(Quick_Test PRIVATE Qt6::Gui Qt6::Quick)
#target_sources(Quick_Test PRIVATE ${SRCS_QUICK_PATH}/qml.qrc) # no need
