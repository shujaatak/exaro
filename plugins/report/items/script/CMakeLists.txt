# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Script)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Script_SRCS script.cpp)
QT4_WRAP_CPP(Script_SRCS script.h)
QT4_ADD_RESOURCES(Script_SRCS script.qrc)

add_library(Script SHARED ${Script_SRCS})
add_dependencies(Script Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Script Report ${QT_LIBRARIES})

install(TARGETS Script ${INSTALL_PLUGINS_DEFAULT_ARGS})
