# cspell: disable

cmake_minimum_required(VERSION 3.19)
project(CAHD LANGUAGES CXX)

set(OpenCASCADE_DIR "" CACHE PATH "OpenCascade's cmake directory, default should work on Linux, see 'README.md' 'Build' section for more information.")
set(THIRD_PARTY_DIR "" CACHE PATH "OpenCascade third party libs directory, default should work on Linux, see 'README.md' 'Build' section for more information.")

if (WIN32)
    if (OpenCASCADE_DIR STREQUAL "")
        message(WARNING "OpenCASCADE_DIR is not configured, trying nonetheless...")
    endif (OpenCASCADE_DIR STREQUAL "")
    if (THIRD_PARTY_DIR STREQUAL "")
        message(WARNING "THIRD_PARTY_DIR is not configured, trying nonetheless...")
    endif (THIRD_PARTY_DIR STREQUAL "")
endif (WIN32)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets)
find_package(OpenCASCADE REQUIRED COMPONENTS FoundationClasses ModelingData ModelingAlgorithms Visualization ApplicationFramework DataExchange)

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_add_executable(CAHD
    MACOSX_BUNDLE
    main.cpp
    modelinfo.h
    modelinfo.cpp
    mainwindow/mainwindow.cpp
    mainwindow/mainwindow.h
    mainwindow/mainwindow.ui
    occtwidget/occtwidget.cpp
    occtwidget/occtwidget.h
)

target_include_directories(CAHD
    PRIVATE
        .)

target_link_libraries(CAHD
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Gui
        Qt::OpenGL
        Qt::OpenGLWidgets
        # OpenCASCADE
        TKBRep
        TKDESTEP
        TKG3d
        TKGeomAlgo
        TKGeomBase
        TKMath
        TKOpenGl
        TKPrim
        TKTopAlgo
        TKV3d
)

add_custom_command(TARGET CAHD POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/../lib/openvr_api.dll"
        $<TARGET_FILE_DIR:CAHD>)  

include(GNUInstallDirs)
