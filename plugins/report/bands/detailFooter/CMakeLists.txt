# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(DetailFooter)

include_directories(BEFORE ../../../../libs/report)

set(DetailFooter_SRCS detailfooter.cpp)

QT4_WRAP_CPP(DetailFooter_SRCS detailfooter.h)

QT4_ADD_RESOURCES(DetailFooter_SRCS detailFooter.qrc)

add_library(DetailFooter SHARED ${DetailFooter_SRCS})
add_dependencies(DetailFooter Report)
link_directories( " ../../../../libs/report" )
target_link_libraries(DetailFooter Report ${QT_LIBRARIES})

install(TARGETS DetailFooter LIBRARY DESTINATION lib/report)
