# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >
# (c) 2009 by Mikhalov Alexander < alexmi3@rambler.ru >

cmake_minimum_required(VERSION 2.6)
project(exaro-package)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set(CMAKE_VERBOSE_MAKEFILE ON)
#comment or remove the above line before release

# please choose between GPL or LGPL license
set (EXARO_LICENSE "GPL")

message("*** eXaro use ${EXARO_LICENSE} license ***")

set (EXARO_VERSION_MAJOR 2)
set (EXARO_VERSION_MINOR 70)
set (EXARO_VERSION_RELEASE 0)
set (EXARO_VERSION "${EXARO_VERSION_MAJOR}.${EXARO_VERSION_MINOR}.${EXARO_VERSION_RELEASE}" )


add_definitions (-DEXARO_VERSION=\"${EXARO_VERSION}\")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )

include(SetPaths.cmake)

add_definitions (-DEXARO_PLUGINS_PATH=\"${LIB_INSTALL_DIR}\")



if(UNIX)
  find_package(SharedMimeInfo REQUIRED)
endif(UNIX)

find_package(Qt4 REQUIRED)
set( QT_USE_QTGUI TRUE )
set( QT_USE_QTUITOOLS TRUE )
set( QT_USE_QTXML TRUE )
set( QT_USE_QTSQL TRUE )
set( QT_USE_QTSCRIPT TRUE )
set( QT_USE_QTWEBKIT TRUE )
set( QT_USE_QTOPENGL TRUE )

include( ${QT_USE_FILE} )
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR} 
        ${QT_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        )
add_subdirectory(libs)
add_subdirectory(plugins)

find_package(PropertyEditor)
if (PROPERTYEDITOR_FOUND)
    add_subdirectory(apps)
else(PROPERTYEDITOR_FOUND)
    message("** propertyEditor was not found in your system, if you want to use eXaro designer, consider to download and install propertyEditor from \"http://code.google.com/p/propertyeditor\"")
endif(PROPERTYEDITOR_FOUND)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

install(FILES cmake/modules/FindExaro.cmake DESTINATION ${CMAKE_ROOT}/Modules COMPONENT Devel)
