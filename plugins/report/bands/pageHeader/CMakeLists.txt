# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(PageHeader)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(PageHeader_SRCS pageheader.cpp)
QT4_WRAP_CPP(PageHeader_SRCS pageheader.h)
QT4_ADD_RESOURCES(PageHeader_SRCS pageHeader.qrc)

add_library(PageHeader SHARED ${PageHeader_SRCS})
add_dependencies(PageHeader Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(PageHeader Report ${QT_LIBRARIES})

install(TARGETS PageHeader ${INSTALL_PLUGINS_DEFAULT_ARGS})
