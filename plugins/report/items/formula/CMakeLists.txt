# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Formula)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Formula_SRCS formula.cpp)
QT4_WRAP_CPP(Formula_SRCS formula.h)
QT4_ADD_RESOURCES(Formula_SRCS formula.qrc)

add_library(Formula SHARED ${Formula_SRCS})
add_dependencies(Formula Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Formula Report ${QT_LIBRARIES})

install(TARGETS Formula ${INSTALL_PLUGINS_DEFAULT_ARGS})
