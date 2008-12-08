# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Overlay)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Overlay_SRCS overlay.cpp)
QT4_WRAP_CPP(Overlay_SRCS overlay.h)
QT4_ADD_RESOURCES(Overlay_SRCS overlay.qrc)

add_library(Overlay SHARED ${Overlay_SRCS})
add_dependencies(Overlay Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Overlay Report ${QT_LIBRARIES})

install(TARGETS Overlay ${INSTALL_PLUGINS_DEFAULT_ARGS})
