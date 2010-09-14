# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Field)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Field_SRCS field.cpp)
QT4_WRAP_CPP(Field_SRCS field.h)
QT4_ADD_RESOURCES(Field_SRCS field.qrc)

add_library(Field SHARED ${Field_SRCS})
add_dependencies(Field Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Field Report ${QT_LIBRARIES})

install(TARGETS Field ${INSTALL_PLUGINS_DEFAULT_ARGS})
