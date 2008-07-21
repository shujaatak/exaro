TEMPLATE = lib

CONFIG += exceptions \
dll \
plugin \
  rtti


HEADERS += strings.h

SOURCES += strings.cpp

TARGET = StringsProperty

DESTDIR = .

INCLUDEPATH += $$[EXARO_INSTALL_HEADERS]

macx{
	QMAKE_LFLAGS +=  -F$$[EXARO_INSTALL_LIBS]
 	LIBS += -framework PropertyEditor
}else{
	LIBS += -L$$[EXARO_INSTALL_LIBS] -lPropertyEditor 
}


target.path = $$[EXARO_INSTALL_PLUGINS]/popertyEditor

INSTALLS += target

