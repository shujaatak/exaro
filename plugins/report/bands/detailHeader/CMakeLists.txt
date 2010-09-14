# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(DetailHeader)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(DetailHeader_SRCS detailheader.cpp)
QT4_WRAP_CPP(DetailHeader_SRCS detailheader.h)
QT4_ADD_RESOURCES(DetailHeader_SRCS detailHeader.qrc)

add_library(DetailHeader SHARED ${DetailHeader_SRCS})
add_dependencies(DetailHeader Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(DetailHeader Report ${QT_LIBRARIES})

install(TARGETS DetailHeader ${INSTALL_PLUGINS_DEFAULT_ARGS})
