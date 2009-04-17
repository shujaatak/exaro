# (c) 2009 by Florina Nacu 

project(Piechart)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report" "${CMAKE_SOURCE_DIR}/plugins/report/items/charts/chartinterface")

set(Piechart_SRCS piechart.cpp)
QT4_WRAP_CPP(Piechart_SRCS piechart.h)
QT4_ADD_RESOURCES(Piechart_SRCS piechart.qrc)


add_library(Piechart SHARED ${Piechart_SRCS})
add_dependencies(Piechart ChartInterface)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" "../chartinterface")
target_link_libraries(Piechart ChartInterface Report ${QT_LIBRARIES})

install(TARGETS Piechart ${INSTALL_PLUGINS_DEFAULT_ARGS})
