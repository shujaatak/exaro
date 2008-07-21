TEMPLATE = lib

CONFIG += dll \
plugin \
exceptions \
  rtti \
 uitools

RESOURCES += barcode.qrc

TARGET = Barcode

SOURCES += barcode.cpp

HEADERS += barcode.h

QT += sql \
xml \
script

DESTDIR = .

INCLUDEPATH += $$[EXARO_INSTALL_HEADERS] $$[ZINT_INSTALL_HEADERS]

macx{
	QMAKE_LFLAGS +=  -F$$[EXARO_INSTALL_LIBS] -F$$[ZINT_INSTALL_LIBS] 
 	LIBS += -framework Report -framework QtZint
}else{
	LIBS += -L$$[EXARO_INSTALL_LIBS] -lReport -L$$[ZINT_INSTALL_LIBS] -lQtZint
}

target.path = $$[EXARO_INSTALL_PLUGINS]/report

INSTALLS += target

