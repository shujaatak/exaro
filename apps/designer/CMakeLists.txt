project(exaro)

add_subdirectory(plugins)
include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report" ${PROPERTYEDITOR_INCLUDE_DIR})

set(exaro_SRCS  aboutdialog.cpp          exaroapplication.cpp  scripteditdialog.cpp
		designerpage.cpp         main.cpp              scriptsyntaxhighlighter.cpp
		designerquerywidget.cpp  mainwindow.cpp        sqldatabasedialog.cpp
		designeruitype.cpp       objectmodel.cpp       sqlsyntaxhighlighter.cpp
		designeruiwidget.cpp     queryeditdialog.cpp	optionsdialog.cpp 
		namevalidator.cpp 	 command.cpp 		reportwizard.cpp
		querywizardpage.cpp 	 pagewizardpage.cpp	groupwizardpage.cpp
		fieldswizardpage.cpp)

QT4_WRAP_CPP(exaro_SRCS aboutdialog.h          	designeruiwidget.h  	queryeditdialog.h
			sqlsyntaxhighlighter.h	designerpage.h         	exaroapplication.h
			scripteditdialog.h	designerquerywidget.h  	mainwindow.h
			designeruitype.h       	objectmodel.h       	sqldatabasedialog.h
			namevalidator.h 	optionsdialog.h 	scriptsyntaxhighlighter.h
			reportwizard.h		querywizardpage.h 	pagewizardpage.h
			groupwizardpage.h	fieldswizardpage.h)

QT4_WRAP_UI(exaro_SRCS 	aboutdialog.ui          designeruitype.ui    	mainwindow.ui
			designerquerywidget.ui  designeruiwidget.ui  	sqldatabasedialog.ui
			optionsdialog.ui 	querywizardpage.ui 	pagewizardpage.ui
			groupwizardpage.ui	fieldswizardpage.ui	exceptiondialog.ui)

QT4_ADD_RESOURCES(exaro_SRCS report.qrc)

if(WIN32)
  if(MINGW)
    # resource compilation for mingw
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/exaro_rc.o
                       COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR}
                                           -i${CMAKE_CURRENT_SOURCE_DIR}/exaro.rc
                                           -o ${CMAKE_CURRENT_BINARY_DIR}/exaro_rc.o)
    SET(exaro_SRCS ${exaro_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/exaro_rc.o)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
  else(MINGW)
    SET(exaro_SRCS ${exaro_SRCS} exaro.rc)
  endif(MINGW)
endif(WIN32)

file(GLOB TRANSLATIONS_FILES "${CMAKE_SOURCE_DIR}/translations/*.ts")
qt4_add_translation(QM_FILES ${TRANSLATIONS_FILES})
if (APPLE)
	add_executable(eXaro MACOSX_BUNDLE ${exaro_SRCS} ${QM_FILES})
	add_dependencies(eXaro Report)
	set(MACOSX_BUNDLE_ICON_FILE "${CMAKE_SOURCE_DIR}/apps/designer/eXaro.icns")
	set(MACOSX_BUNDLE_BUNDLE_NAME "exaro")
	set(MACOSX_BUNDLE_BUNDLE_VERSION "${EXARO_VERSION}")
	set(MACOSX_BUNDLE_LONG_VERSION_STRING "${EXARO_VERSION}")
else(APPLE)
	add_executable(exaro ${exaro_SRCS} ${QM_FILES})
	add_dependencies(exaro Report)
endif(APPLE)


link_directories( "${CMAKE_BINARY_DIR}/libs/report" )

if (APPLE)
	target_link_libraries(eXaro Report ${QT_LIBRARIES} ${PROPERTYEDITOR_LIBRARY})
	install(TARGETS eXaro BUNDLE DESTINATION bin)
else(APPLE)
	target_link_libraries(exaro Report ${QT_LIBRARIES} ${PROPERTYEDITOR_LIBRARY})
	install(TARGETS exaro RUNTIME DESTINATION bin)
endif(APPLE)

install(FILES ${QM_FILES} DESTINATION "${TRANSLATIONS_INSTALL_DIR}")
install(FILES application-bdrt.png DESTINATION "${ICON_INSTALL_DIR}")
install(FILES bdrt.desktop DESTINATION "${MIME_INSTALL_DIR}/application")
install(FILES bdrt.xml DESTINATION "${XDG_MIME_INSTALL_DIR}/packages")
install(FILES exaro.desktop DESTINATION "${XDG_APPS_INSTALL_DIR}")
