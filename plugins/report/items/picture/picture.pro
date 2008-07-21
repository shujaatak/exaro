TEMPLATE = lib

CONFIG += dll \
plugin \
exceptions \
  rtti \
 uitools

RESOURCES += picture.qrc

TARGET = Picture

SOURCES += picture.cpp

HEADERS += picture.h

QT += sql \
xml \
script

DESTDIR = .

INCLUDEPATH += $$[EXARO_INSTALL_HEADERS] 


macx{
	QMAKE_LFLAGS +=  -F$$[EXARO_INSTALL_LIBS]
 	LIBS += -framework Report
}else{
	LIBS += -L$$[EXARO_INSTALL_LIBS] -lReport 
}

target.path = $$[EXARO_INSTALL_PLUGINS]/report

INSTALLS += target

