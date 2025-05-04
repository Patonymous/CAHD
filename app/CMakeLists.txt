cmake_minimum_required(VERSION 3.19)
project(CAHD LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets)

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_add_executable(CAHD
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow/mainwindow.cpp
    mainwindow/mainwindow.h
    mainwindow/mainwindow.ui
    occtwidget/occtwidget.cpp
    occtwidget/occtwidget.h
)

target_link_libraries(CAHD
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Gui
        Qt::OpenGL
        Qt::OpenGLWidgets
)

include(GNUInstallDirs)
