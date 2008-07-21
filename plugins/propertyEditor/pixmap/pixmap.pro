TEMPLATE = lib

CONFIG += exceptions \
dll \
plugin \
  rtti

FORMS += pixmapDialog.ui

SOURCES += pixmapdialog.cpp \
pixmap.cpp
HEADERS += pixmapdialog.h \
pixmap.h

TARGET = PixmapProperty

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

