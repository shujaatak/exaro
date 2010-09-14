# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Pie)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Pie_SRCS pie.cpp)
QT4_WRAP_CPP(Pie_SRCS pie.h)
QT4_ADD_RESOURCES(Pie_SRCS pie.qrc)

add_library(Pie SHARED ${Pie_SRCS})
add_dependencies(Pie Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Pie Report ${QT_LIBRARIES})

install(TARGETS Pie ${INSTALL_PLUGINS_DEFAULT_ARGS})
