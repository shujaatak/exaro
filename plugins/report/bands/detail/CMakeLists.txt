# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Detail)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Detail_SRCS detail.cpp)
QT4_WRAP_CPP(Detail_SRCS detail.h)
QT4_ADD_RESOURCES(Detail_SRCS detail.qrc)

add_library(Detail SHARED ${Detail_SRCS})
add_dependencies(Detail Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Detail Report ${QT_LIBRARIES})

install(TARGETS Detail ${INSTALL_PLUGINS_DEFAULT_ARGS})
