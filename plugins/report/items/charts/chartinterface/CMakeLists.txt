# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(ChartInterface)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report" "${CMAKE_SOURCE_DIR}/plugins/report/items/charts/chartcontainer")

set(ChartInterface_SRCS chartdatacollector.cpp  chartinterface.cpp)
QT4_WRAP_CPP(ChartInterface_SRCS chartdatacollector.h  chartinterface.h)


add_library(ChartInterface STATIC ${ChartInterface_SRCS})

if (UNIX)
    set_target_properties(ChartInterface PROPERTIES COMPILE_FLAGS -fPIC)
endif(UNIX)

add_dependencies(ChartInterface Report)
