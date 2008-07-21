TEMPLATE = lib

CONFIG += exceptions \
dll \
plugin \
  rtti

INCLUDEPATH += $$[EXARO_INSTALL_HEADERS]

TARGET = BoolProperty

macx{
	QMAKE_LFLAGS +=  -F$$[EXARO_INSTALL_LIBS]
 	LIBS += -framework PropertyEditor
}else{
	LIBS += -L$$[EXARO_INSTALL_LIBS] -lPropertyEditor 
}

DESTDIR =.

HEADERS += bool.h

SOURCES += bool.cpp

RESOURCES += resource.qrc

target.path = $$[EXARO_INSTALL_PLUGINS]/popertyEditor

INSTALLS += target

