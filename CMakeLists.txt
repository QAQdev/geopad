cmake_minimum_required(VERSION 3.14)
project(GeoPad)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(thirdparty/imgui)
include_directories(thirdparty/dist)
include_directories(thirdparty)
link_directories(lib)

add_executable(GeoPad
        thirdparty/imgui/imgui.cpp
        thirdparty/imgui/imgui_draw.cpp
        thirdparty/imgui/imgui_demo.cpp
        thirdparty/imgui/imgui_impl_glfw.cpp
        thirdparty/imgui/imgui_impl_opengl3.cpp
        thirdparty/imgui/imgui_tables.cpp
        thirdparty/imgui/imgui_widgets.cpp
        thirdparty/dist/jsoncpp.cpp
        main.cpp
        thirdparty/glad.c
        src/include/Config.h
        src/Circle.cpp
        src/Point.cpp
        src/Rect.cpp
        src/Line.cpp
        src/Canvas.cpp
        src/ShapeCreater.cpp
        src/Usr.cpp src/include/Usr.h src/Polygon.cpp src/include/Polygon.h src/IO.cpp src/include/IO.h)

target_link_libraries(GeoPad glfw3 opengl32)

# set(CMAKE_EXE_LINKER_FLAGS -static)
