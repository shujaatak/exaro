project(Fonttoolbar)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Fonttoolbar_SRCS fonttoolbar.cpp)
QT4_WRAP_CPP(Fonttoolbar_SRCS fonttoolbar.h)
QT4_ADD_RESOURCES(Fonttoolbar_SRCS fonttoolbar.qrc)

add_library(Fonttoolbar SHARED ${Fonttoolbar_SRCS})
add_dependencies(Fonttoolbar Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" "../lib")

target_link_libraries(Fonttoolbar Report ${QT_LIBRARIES})

install(TARGETS Fonttoolbar ${INSTALL_EXARO_TOOLBAR_PLUGINS_DEFAULT_ARGS})
