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

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Xml)
find_package(Qt6 REQUIRED COMPONENTS Gui)

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

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qrc_Editor MANUAL_FINALIZATION ${PROJECT_SOURCES})
    # Define target properties for Android with Qt 6 as: set_property(TARGET
    # qrc_Editor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    # ${CMAKE_CURRENT_SOURCE_DIR}/android) For more information, see
    # https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qrc_Editor SHARED ${PROJECT_SOURCES})
        # Define properties for Android with Qt 5 after find_package() calls as:
        # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qrc_Editor ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(qrc_Editor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(qrc_Editor PRIVATE Qt6::Xml)
target_link_libraries(qrc_Editor PRIVATE Qt6::Gui)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If
# you are developing for iOS or macOS you should consider setting an explicit,
# fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qrc_Editor)
endif()
set_target_properties(
    qrc_Editor
    PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
               ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
               ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
               TRUE WIN32_EXECUTABLE
               TRUE)

include(GNUInstallDirs)
install(
    TARGETS qrc_Editor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qrc_Editor)
endif()
