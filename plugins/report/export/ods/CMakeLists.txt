# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Ods)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report" ${QUAZIP_INCLUDE_DIR})

set(Ods_SRCS ods.cpp)
QT4_WRAP_CPP(Ods_SRCS ods.h)
QT4_ADD_RESOURCES(Ods_SRCS odsResource.qrc)


add_library(Ods SHARED ${Ods_SRCS})
add_dependencies(Ods Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Ods Report ${QT_LIBRARIES} ${QUAZIP_LIBRARY})

install(TARGETS Ods ${INSTALL_PLUGINS_EXPORT_DEFAULT_ARGS})
