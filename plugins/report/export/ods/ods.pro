TEMPLATE = lib

CONFIG += dll \
plugin \
exceptions \
  rtti \
 uitools

TARGET = Ods

QT += sql \
xml \
script

RESOURCES += odsResource.qrc

HEADERS += ods.h

SOURCES += ods.cpp

DESTDIR = .

INCLUDEPATH += $$[EXARO_INSTALL_HEADERS] $$[QUAZIP_INSTALL_HEADERS]

macx{
	QMAKE_LFLAGS +=  -F$$[EXARO_INSTALL_LIBS] -F$$[QUAZIP_INSTALL_LIBS]
 	LIBS += -framework Report -framework quazip
}else{
	LIBS += -L$$[EXARO_INSTALL_LIBS] -lReport -lquazip
}

target.path = $$[EXARO_INSTALL_PLUGINS]/report/export
INSTALLS += target
