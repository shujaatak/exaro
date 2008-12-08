# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Line)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Line_SRCS line.cpp)
QT4_WRAP_CPP(Line_SRCS line.h)
QT4_ADD_RESOURCES(Line_SRCS line.qrc)

add_library(Line SHARED ${Line_SRCS})
add_dependencies(Line Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Line ${QT_LIBRARIES} Report)

install(TARGETS Line ${INSTALL_PLUGINS_DEFAULT_ARGS})
