TEMPLATE =   lib

CONFIG += dll \
exceptions \
  rtti \
 uitools

macx{
	CONFIG -= dll
	CONFIG += lib_bundle
}

win32 : DEFINES += LIB_EXPORTS

SOURCES +=   iteminterface.cpp \
  pageinterface.cpp \
  reportinterface.cpp \
  bandinterface.cpp \
   sqlquery.cpp \
   reportengine.cpp \
   globals.cpp \
   paintdevice.cpp \
   paintengine.cpp \
   document.cpp \
   page.cpp \
   previewdialog.cpp \
   previewwidget.cpp \
   searchwidget.cpp \
   exportinterface.cpp \
   pagegraphicsitem.cpp

HEADERS +=   iteminterface.h \
  pageinterface.h \
  reportinterface.h \
  bandinterface.h \
   sqlquery.h \
   reportengine.h \
   globals.h \
   paintdevice.h \
   paintengine.h \
   document.h \
   page.h \
   previewdialog.h \
   previewwidget.h \
   searchwidget.h \
   exportinterface.h \
   pagegraphicsitem.h

QT += script \
 sql \
 xml

TARGET = Report

RESOURCES += preview.qrc

DESTDIR = .

include.path = $$[EXARO_INSTALL_HEADERS]
include.files = bandinterface.h exportinterface.h  iteminterface.h  pageinterface.h  reportinterface.h reportengine.h globals.h
target.path = $$[EXARO_INSTALL_LIBS]

INSTALLS += include target


