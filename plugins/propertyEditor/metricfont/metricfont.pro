TEMPLATE = lib

CONFIG += exceptions \
dll \
plugin \
  rtti

HEADERS += font.h \
changefont.h \
 metricfontdialog.h

SOURCES += font.cpp \
changefont.cpp \
 metricfontdialog.cpp

TARGET = FontProperty

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

FORMS += metricfontdialog.ui

