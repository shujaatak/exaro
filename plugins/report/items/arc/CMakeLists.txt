# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Arc)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Arc_SRCS arc.cpp)
QT4_WRAP_CPP(Arc_SRCS arc.h)
QT4_ADD_RESOURCES(Arc_SRCS arc.qrc)

add_library(Arc SHARED ${Arc_SRCS})
add_dependencies(Arc Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Arc Report ${QT_LIBRARIES})

install(TARGETS Arc ${INSTALL_PLUGINS_DEFAULT_ARGS})
