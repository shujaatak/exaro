# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Picture)

include_directories(BEFORE ../../../../libs/report)

set(Picture_SRCS picture.cpp)

QT4_WRAP_CPP(Picture_SRCS picture.h)

QT4_ADD_RESOURCES(Picture_SRCS picture.qrc)

add_library(Picture SHARED ${Picture_SRCS})
add_dependencies(Picture Report)
link_directories( " ../../../../libs/report" )
target_link_libraries(Picture Report ${QT_LIBRARIES})

install(TARGETS Picture DESTINATION "${PLUGIN_INSTALL_DIR}" RUNTIME)
