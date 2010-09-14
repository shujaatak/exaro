# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Text)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Text_SRCS text.cpp)
QT4_WRAP_CPP(Text_SRCS text.h)
QT4_ADD_RESOURCES(Text_SRCS text.qrc)

add_library(Text SHARED ${Text_SRCS})
add_dependencies(Text Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Text Report ${QT_LIBRARIES})

install(TARGETS Text ${INSTALL_PLUGINS_DEFAULT_ARGS})
