# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

cmake_minimum_required(VERSION 2.6)
project(eXaro_Music_Example)

find_package(Qt4 REQUIRED)
set( QT_USE_QTGUI TRUE )
#set( QT_USE_QTUITOOLS TRUE )
#set( QT_USE_QTXML TRUE )
set( QT_USE_QTSQL TRUE )
set( QT_USE_QTSCRIPT TRUE )
#set( QT_USE_QTWEBKIT TRUE )
#set( QT_USE_QTOPENGL TRUE )

find_package(Exaro REQUIRED)

include( ${QT_USE_FILE} )
#include_directories(
#        ${CMAKE_CURRENT_SOURCE_DIR}
#        ${QT_INCLUDE_DIR}
#        ${CMAKE_CURRENT_BINARY_DIR}
#        )

include_directories(${EXARO_INCLUDE_DIR} ${QT_INCLUDE_DIR})

add_executable(musicExample main.cpp)
target_link_libraries(musicExample ${EXARO_LIBRARIES} ${QT_LIBRARIES})