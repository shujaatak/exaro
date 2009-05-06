# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

cmake_minimum_required(VERSION 2.6)
project(exaro-package)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set(CMAKE_VERBOSE_MAKEFILE ON)
#comment or remove the above line before release

# please choose between GPL or LGPL license
set (EXARO_LICENSE "GPL")

message("*** eXaro use ${EXARO_LICENSE} license ***")

set (EXARO_VERSION_MAJOR 1)
set (EXARO_VERSION_MINOR 85)
set (EXARO_VERSION_RELEASE 0)
set (EXARO_VERSION "${EXARO_VERSION_MAJOR}.${EXARO_VERSION_MINOR}.${EXARO_VERSION_RELEASE}" )


add_definitions (-DEXARO_VERSION=\"${EXARO_VERSION}\")

INCLUDE (CheckCXXCompilerFlag)

check_cxx_compiler_flag("-Wall" CXX_COMPILER_FLAG_WALL)
if (CXX_COMPILER_FLAG_WALL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif (CXX_COMPILER_FLAG_WALL)

check_cxx_compiler_flag("-fexceptions" CXX_COMPILER_FLAG_FEXCEPTIONS)
if (CXX_COMPILER_FLAG_FEXCEPTIONS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
endif (CXX_COMPILER_FLAG_FEXCEPTIONS)

check_cxx_compiler_flag("-fnon-call-exceptions" CXX_COMPILER_FLAG_FNON_CALL_EXCEPTIONS)
if (CXX_COMPILER_FLAG_FNON_CALL_EXCEPTIONS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fnon-call-exceptions")
endif (CXX_COMPILER_FLAG_FNON_CALL_EXCEPTIONS)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )

include(SetPaths.cmake)

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
#set( QT_USE_QTOPENGL TRUE )

include( ${QT_USE_FILE} )
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR} 
        ${QT_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        )

add_definitions (-DLIB_INSTALL_DIR=\"${LIB_INSTALL_DIR}\")

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

#file (GLOB TRANSLATIONS_FILES translations/*.ts)
#qt4_add_translation(QM_FILES ${TRANSLATIONS_FILES})

#install(FILES ${QM_FILES} DESTINATION "${TRANSLATIONS_INSTALL_DIR}")
