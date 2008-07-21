TEMPLATE = lib

CONFIG += exceptions \
dll \
plugin \
  rtti

HEADERS += flags.h \
 flagslist.h

SOURCES += flags.cpp \
 flagslist.cpp

FORMS += flagsList.ui

TARGET = FlagsProperty

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

