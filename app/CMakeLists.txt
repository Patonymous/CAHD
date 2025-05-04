cmake_minimum_required(VERSION 3.19)
project(CAHD LANGUAGES CXX)

# Load environment variables from build.env
file(STRINGS ${CMAKE_HOME_DIRECTORY}/build.env ENV_FILE)
foreach(VAR ${ENV_FILE})
    string(REGEX MATCH "^[^=]*" KEY ${VAR})
    string(REGEX REPLACE "^[^=]*=" "" VALUE ${VAR})
    set(ENV{${KEY}} ${VALUE})
endforeach()

set(OpenCASCADE_DIR $ENV{OPEN_CASCADE_DIR}/cmake)
set(THIRD_PARTY_DIR $ENV{THIRD_PARTY_DIR})

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets)
find_package(OpenCASCADE REQUIRED COMPONENTS FoundationClasses ModelingData ModelingAlgorithms Visualization ApplicationFramework DataExchange)

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
        # OpenCASCADE
        TKV3d
)

configure_file(
   ../lib/openvr_api.dll openvr_api.dll COPYONLY
)

include(GNUInstallDirs)
