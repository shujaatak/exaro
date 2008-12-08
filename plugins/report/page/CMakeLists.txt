# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Page)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Page_SRCS page.cpp)
QT4_WRAP_CPP(Page_SRCS page.h)
QT4_ADD_RESOURCES(Page_SRCS page.qrc)

add_library(Page SHARED ${Page_SRCS})
add_dependencies(Page Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Page Report ${QT_LIBRARIES})

install(TARGETS Page ${INSTALL_PLUGINS_DEFAULT_ARGS})
