# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Barchart)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report" "${CMAKE_SOURCE_DIR}/plugins/report/items/charts/chartinterface")

set(Barchart_SRCS barchart.cpp)
QT4_WRAP_CPP(Barchart_SRCS barchart.h)
QT4_ADD_RESOURCES(Barchart_SRCS barchart.qrc)


add_library(Barchart SHARED ${Barchart_SRCS})
add_dependencies(Barchart ChartInterface)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" "../chartinterface")
target_link_libraries(Barchart ChartInterface Report ${QT_LIBRARIES})

install(TARGETS Barchart ${INSTALL_PLUGINS_DEFAULT_ARGS})
