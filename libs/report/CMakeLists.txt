# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(Report)

set(Report_SRCS bandinterface.cpp    iteminterface.cpp     paintdevice.cpp    reportengine.cpp
		document.cpp         page.cpp              paintengine.cpp    reportinterface.cpp
		exportinterface.cpp  pagegraphicsitem.cpp  previewdialog.cpp  searchwidget.cpp
		globals.cpp          pageinterface.cpp     previewwidget.cpp  sqlquery.cpp)

QT4_WRAP_CPP(Report_SRCS bandinterface.h    iteminterface.h    reportengine.h
			reportinterface.h
			exportinterface.h  previewdialog.h  searchwidget.h
			pageinterface.h     previewwidget.h  sqlquery.h)

QT4_ADD_RESOURCES(Report_SRCS preview.qrc)

add_library(Report SHARED ${Report_SRCS})

set_target_properties(Report PROPERTIES	VERSION ${EXARO_VERSION}
                                        SOVERSION ${EXARO_VERSION})

target_link_libraries(Report ${QT_LIBRARIES})

install(TARGETS Report LIBRARY DESTINATION lib)
install(FILES bandinterface.h exportinterface.h  iteminterface.h  pageinterface.h  reportinterface.h reportengine.h globals.h sqlquery.h DESTINATION include COMPONENT Devel)
