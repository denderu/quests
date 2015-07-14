#CMakeLists.txt for Qt5 projects
#
# Main changes to Qt4: target_link_libraries, include_directories,
# and add_definitions is not needed anymore and replaced by
# qt5_use_modules.
# QT5_WRAP_CPP is replaced by set(CMAKE_AUTOMOC ON).

cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_PREFIX_PATH "C:/Qt/5.4/msvc2010_opengl")

#Set project name
set(PROJECT_NAME PlayerFullScreen)
project(${PROJECT_NAME})

# Find the QtWidgets library
find_package(Qt5Widgets)


# Assure that moc is run when needed. Removing this line, you will
# have to run moc manually or get linker errors.
set(CMAKE_AUTOMOC ON)
set(QT_USE_QTNETWORK TRUE)

# This line includes the current Dir of CMake. The xxx.ui file is
# converted to a ui_xxx.h file by uic.exe. The header ui_xxx.h file
# is located in the build directory. To make this header visible to
# MSVS the build path needs to be included.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Set sources and headers and forms
set(SOURCES src/main.cpp src/mainwindow.cpp ../Client/src/clientmodel.cpp)
set(HEADERS src/mainwindow.h ../Client/src/clientmodel.h)
set(FORMS src/mainwindow.ui)

#set(RESOURCES src/xxx.rc)

#This line is necessary to cal uic.exe and creates the ui_xxx.h
#files from the xxx.ui files. The header is created the first time
#the project is compiled
QT5_WRAP_UI(FORMS_HEADERS ${FORMS})

#Add all source files to the executable
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS} ${FORMS_HEADERS} ${HEADERS_MOC})

target_link_libraries(${PROJECT_NAME} ${Qt5Core_QTMAIN_LIBRARIES}  ${Qt5_QTNETWORK_LIBRARY})

#Qt5 macro encapsulates all of the variable usage required to use a
#Qt module.
qt5_use_modules(${PROJECT_NAME} Widgets Network Multimedia MultimediaWidgets)