# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Html)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Html_SRCS html.cpp)
QT4_WRAP_CPP(Html_SRCS html.h)

add_library(Html SHARED ${Html_SRCS})
add_dependencies(Html Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Html Report ${QT_LIBRARIES})

install(TARGETS Html  ${INSTALL_PLUGINS_EXPORT_DEFAULT_ARGS})
