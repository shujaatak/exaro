# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Title)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Title_SRCS title.cpp)
QT4_WRAP_CPP(Title_SRCS title.h)
QT4_ADD_RESOURCES(Title_SRCS title.qrc)

add_library(Title SHARED ${Title_SRCS})
add_dependencies(Title Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Title Report ${QT_LIBRARIES})

install(TARGETS Title ${INSTALL_PLUGINS_DEFAULT_ARGS})
