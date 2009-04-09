project(SqlDataset)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

#set(SqlDataset_SRCS sqldataset.cpp sqlsyntaxhighlighter.cpp)
set(SqlDataset_SRCS sqldataset.cpp sqlsyntaxhighlighter.cpp sqldataseteditor.cpp)
#QT4_WRAP_CPP(SqlDataset_SRCS sqldataset.h sqlsyntaxhighlighter.h)
QT4_WRAP_CPP(SqlDataset_SRCS sqldataset.h sqlsyntaxhighlighter.h sqldataseteditor.h)
QT4_WRAP_UI(SqlDataset_SRCS sqldataseteditor.ui)


add_library(SqlDataset SHARED ${SqlDataset_SRCS})
add_dependencies(SqlDataset Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(SqlDataset Report ${QT_LIBRARIES})

install(TARGETS SqlDataset  ${INSTALL_PLUGINS_DATASET_DEFAULT_ARGS})
