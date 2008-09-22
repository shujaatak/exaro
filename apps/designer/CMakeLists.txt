# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(exaro)

include_directories(BEFORE ../../libs/report ${PROPERTYEDITOR_INCLUDE_DIR})

set(exaro_SRCS aboutdialog.cpp          exaroapplication.cpp  scripteditdialog.cpp
		designerpage.cpp         main.cpp              scriptsyntaxhighlighter.cpp
		designerquerywidget.cpp  mainwindow.cpp        sqldatabasedialog.cpp
		designeruitype.cpp       objectmodel.cpp       sqlsyntaxhighlighter.cpp
		designeruiwidget.cpp     queryeditdialog.cpp)

QT4_WRAP_CPP(exaro_SRCS aboutdialog.h          designeruiwidget.h  queryeditdialog.h          sqlsyntaxhighlighter.h
			designerpage.h         exaroapplication.h  scripteditdialog.h
			designerquerywidget.h  mainwindow.h        scriptsyntaxhighlighter.h
			designeruitype.h       objectmodel.h       sqldatabasedialog.h)

QT4_WRAP_UI(exaro_SRCS aboutdialog.ui          designeruitype.ui    mainwindow.ui
			designerquerywidget.ui  designeruiwidget.ui  sqldatabasedialog.ui)

QT4_ADD_RESOURCES(exaro_SRCS report.qrc)

add_executable(exaro ${exaro_SRCS})
add_dependencies(exaro Report)
link_directories( " ../../libs/report" )
target_link_libraries(exaro Report ${QT_LIBRARIES} ${PROPERTYEDITOR_LIBRARY})

install(TARGETS exaro RUNTIME DESTINATION bin)
