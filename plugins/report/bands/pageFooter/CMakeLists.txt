# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(PageFooter)

include_directories(BEFORE ../../../../libs/report)

set(PageFooter_SRCS pagefooter.cpp)

QT4_WRAP_CPP(PageFooter_SRCS pagefooter.h)

QT4_ADD_RESOURCES(PageFooter_SRCS pageFooter.qrc)

add_library(PageFooter SHARED ${PageFooter_SRCS})
add_dependencies(PageFooter Report)
link_directories( " ../../../../libs/report" )
target_link_libraries(PageFooter Report ${QT_LIBRARIES})

install(TARGETS PageFooter LIBRARY DESTINATION lib/report)
