project(CsvDataset)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(CsvDataset_SRCS csvdataset.cpp csvdataseteditor.cpp model.cpp)
QT4_WRAP_CPP(CsvDataset_SRCS csvdataset.h csvdataseteditor.h model.h)
QT4_WRAP_UI(CsvDataset_SRCS csvdataseteditor.ui)

add_library(CsvDataset SHARED ${CsvDataset_SRCS})
add_dependencies(CsvDataset Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(CsvDataset Report ${QT_LIBRARIES})

install(TARGETS CsvDataset  ${INSTALL_PLUGINS_DATASET_DEFAULT_ARGS})
