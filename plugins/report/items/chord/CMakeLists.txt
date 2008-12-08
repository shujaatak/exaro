# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Chord)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Chord_SRCS chord.cpp)
QT4_WRAP_CPP(Chord_SRCS chord.h)
QT4_ADD_RESOURCES(Chord_SRCS chord.qrc)

add_library(Chord SHARED ${Chord_SRCS})
add_dependencies(Chord Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Chord Report ${QT_LIBRARIES})

install(TARGETS Chord ${INSTALL_PLUGINS_DEFAULT_ARGS})
