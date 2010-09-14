# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(HtmlScript)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(HtmlScript_SRCS htmlscript.cpp)
QT4_WRAP_CPP(HtmlScript_SRCS htmlscript.h)
QT4_ADD_RESOURCES(HtmlScript_SRCS htmlscript.qrc)

add_library(HtmlScript SHARED ${HtmlScript_SRCS})
add_dependencies(HtmlScript Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(HtmlScript Report ${QT_LIBRARIES})

install(TARGETS HtmlScript ${INSTALL_PLUGINS_DEFAULT_ARGS})
