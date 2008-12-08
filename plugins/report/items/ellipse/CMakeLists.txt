# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Ellipse)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Ellipse_SRCS ellipse.cpp)
QT4_WRAP_CPP(Ellipse_SRCS ellipse.h)
QT4_ADD_RESOURCES(Ellipse_SRCS ellipse.qrc)

add_library(Ellipse SHARED ${Ellipse_SRCS})
add_dependencies(Ellipse Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Ellipse Report ${QT_LIBRARIES})

install(TARGETS Ellipse ${INSTALL_PLUGINS_DEFAULT_ARGS})
