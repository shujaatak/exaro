TEMPLATE = lib

CONFIG += dll \
plugin \
exceptions \
  rtti \
 uitools

TARGET = Html

QT += sql \
xml \
script

HEADERS += html.h

SOURCES += html.cpp


DESTDIR = .

INCLUDEPATH += $$[EXARO_INSTALL_HEADERS]

macx{
	QMAKE_LFLAGS +=  -F$$[EXARO_INSTALL_LIBS]
 	LIBS += -framework Report
}else{
	LIBS += -L$$[EXARO_INSTALL_LIBS] -lReport 
}

target.path = $$[EXARO_INSTALL_PLUGINS]/report/export

INSTALLS += target

