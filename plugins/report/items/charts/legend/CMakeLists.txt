# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Legend)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report" "${CMAKE_SOURCE_DIR}/plugins/report/items/charts/chartinterface")

set(Legend_SRCS legend.cpp)
QT4_WRAP_CPP(Legend_SRCS legend.h)
QT4_ADD_RESOURCES(Legend_SRCS legend.qrc)


add_library(Legend SHARED ${Legend_SRCS})
add_dependencies(Legend ChartInterface)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" "../chartinterface")
target_link_libraries(Legend ChartInterface Report ${QT_LIBRARIES})

install(TARGETS Legend ${INSTALL_PLUGINS_DEFAULT_ARGS})
