# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Summary)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Summary_SRCS summary.cpp)
QT4_WRAP_CPP(Summary_SRCS summary.h)
QT4_ADD_RESOURCES(Summary_SRCS summary.qrc)

add_library(Summary SHARED ${Summary_SRCS})
add_dependencies(Summary Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Summary Report ${QT_LIBRARIES})

install(TARGETS Summary ${INSTALL_PLUGINS_DEFAULT_ARGS})
