# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(DetailContainer)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(DetailContainer_SRCS detailcontainer.cpp)
QT4_WRAP_CPP(DetailContainer_SRCS detailcontainer.h)
QT4_ADD_RESOURCES(DetailContainer_SRCS detailContainer.qrc)

add_library(DetailContainer SHARED ${DetailContainer_SRCS})
add_dependencies(DetailContainer Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(DetailContainer Report ${QT_LIBRARIES})

install(TARGETS DetailContainer ${INSTALL_PLUGINS_DEFAULT_ARGS})
