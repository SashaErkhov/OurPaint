cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

project(ourpaint)
# for using this app, you need to download QT6. If you have question, ask @ardrasss
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable(ourpaint
        main.cpp
        painter/paint.cpp
        painter/BMPpainter.cpp
        painter/objects.cpp
        painter/requirements.cpp
        GUI/mainwindow.cpp
        painter/Painter.h
        painter/QTPainter.h
        painter/QTPainter.cpp
        painter/saveload/objectInFile.cpp
        painter/saveload/FileOurP.cpp
)

include_directories(additions)
include_directories(UndoRedo)
include_directories(BMPfile/code)


add_subdirectory(BMPfile)
add_subdirectory(test)
target_link_libraries(ourpaint bitmap)

target_link_libraries(ourpaint
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)


