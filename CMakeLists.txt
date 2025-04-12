cmake_minimum_required(VERSION 3.5)

project(
        qrc_Editor
        VERSION 0.1
        LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Xml Gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(PROJECT_SOURCES
        main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        src/mainwindow.ui
        src/previewdialog.cpp
        include/previewdialog.h
        src/previewdialog.ui
        src/comboboxDelegate.cpp
        include/comboboxDelegate.h
        src/parser/qrc_parser.cpp
        include/qrc_parser.h
        src/parser/qrc_prefix.cpp
        include/qrc_prefix.h
        src/parser/qrc_file.cpp
        include/qrc_file.h
        resource/icon.rc)

qt_add_executable(qrc_Editor MANUAL_FINALIZATION ${PROJECT_SOURCES})

target_link_libraries(qrc_Editor PRIVATE Qt6::Widgets Qt6::Xml Qt6::Gui)

qt_finalize_executable(qrc_Editor)
