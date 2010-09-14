# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Rectangle)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Rectangle_SRCS rectangle.cpp)
QT4_WRAP_CPP(Rectangle_SRCS rectangle.h)
QT4_ADD_RESOURCES(Rectangle_SRCS rectangle.qrc)

add_library(Rectangle SHARED ${Rectangle_SRCS})
add_dependencies(Rectangle Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Rectangle Report ${QT_LIBRARIES})

install(TARGETS Rectangle ${INSTALL_PLUGINS_DEFAULT_ARGS})
