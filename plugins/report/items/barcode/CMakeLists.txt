# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Barcode)

include_directories(BEFORE ../../../../libs/report ${ZINT_INCLUDE_DIR})

set(Barcode_SRCS barcode.cpp)

QT4_WRAP_CPP(Barcode_SRCS barcode.h)

QT4_ADD_RESOURCES(Barcode_SRCS barcode.qrc)


add_library(Barcode SHARED ${Barcode_SRCS})
add_dependencies(Barcode Report)
link_directories( " ../../../../libs/report" )
target_link_libraries(Barcode Report ${QT_LIBRARIES} ${ZINT_LIBRARY})

install(TARGETS Barcode LIBRARY DESTINATION lib/report)
