# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(CsvTxt)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(CsvTxt_SRCS csvtxt.cpp  exportsettingsdialog.cpp)
QT4_WRAP_CPP(CsvTxt_SRCS csvtxt.h  exportsettingsdialog.h)
QT4_WRAP_UI(CsvTxt_SRCS exportSettingsDialog.ui)

add_library(CsvTxt SHARED ${CsvTxt_SRCS})
add_dependencies(CsvTxt Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(CsvTxt Report ${QT_LIBRARIES})

install(TARGETS CsvTxt  ${INSTALL_PLUGINS_EXPORT_DEFAULT_ARGS})
