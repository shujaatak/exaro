# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(ChartInterface)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report" "${CMAKE_SOURCE_DIR}/plugins/report/items/charts/chartcontainer")

set(ChartInterface_SRCS chartdatacollector.cpp  chartinterface.cpp)
QT4_WRAP_CPP(ChartInterface_SRCS chartdatacollector.h  chartinterface.h)


add_library(ChartInterface STATIC ${ChartInterface_SRCS})
add_dependencies(ChartInterface Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(ChartInterface Report ${QT_LIBRARIES})

#install(TARGETS ChartInterface DESTINATION "${PLUGIN_INSTALL_DIR}" RUNTIME)
