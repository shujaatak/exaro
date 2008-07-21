TEMPLATE = lib

CONFIG +=   exceptions \
dll \
  rtti

macx{
	CONFIG += lib_bundle
}

win32: DEFINES += LIB_EXPORTS

HEADERS += propertyeditor.h \
 propertymodel.h \
 propertyinterface.h \
 propertydelegate.h

SOURCES += propertyeditor.cpp \
 propertymodel.cpp \
 propertyinterface.cpp \
 propertydelegate.cpp


TARGET = PropertyEditor

DESTDIR = .

include.path = $$[EXARO_INSTALL_HEADERS]
include.files = propertyeditor.h propertyinterface.h

target.path = $$[EXARO_INSTALL_LIBS]

INSTALLS += include target

