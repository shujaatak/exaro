# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(ChartContainer)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report" "${CMAKE_SOURCE_DIR}/plugins/report/items/charts/chartinterface")

set(ChartContainer_SRCS chartcontainer.cpp)
QT4_WRAP_CPP(ChartContainer_SRCS chartcontainer.h)
QT4_ADD_RESOURCES(ChartContainer_SRCS chartcontainer.qrc)


add_library(ChartContainer SHARED ${ChartContainer_SRCS})
add_dependencies(ChartContainer ChartInterface)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" "../chartinterface")
target_link_libraries(ChartContainer ChartInterface Report ${QT_LIBRARIES})

install(TARGETS ChartContainer ${INSTALL_PLUGINS_DEFAULT_ARGS})
