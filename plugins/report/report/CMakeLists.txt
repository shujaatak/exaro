# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(StdReport)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(StdReport_SRCS report.cpp)
QT4_WRAP_CPP(StdReport_SRCS report.h)

add_library(StdReport SHARED ${StdReport_SRCS})
add_dependencies(StdReport Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(StdReport Report ${QT_LIBRARIES})

install(TARGETS StdReport ${INSTALL_PLUGINS_DEFAULT_ARGS})
