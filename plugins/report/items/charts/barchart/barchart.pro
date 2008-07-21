TEMPLATE = lib

CONFIG += dll \
plugin \
exceptions \
  rtti \
 uitools


TARGET = BarChart

RESOURCES += barchart.qrc

HEADERS += barchart.h

SOURCES += barchart.cpp

QT += sql \
xml \
script

DESTDIR = .


macx {
    QMAKE_LFLAGS +=  -F$$[EXARO_INSTALL_LIBS]
    LIBS += -framework Report
}else {
    LIBS += -L$$[EXARO_INSTALL_LIBS] -lReport 
}

LIBS += ../chartinterface/libchartinterface.a

target.path = $$[EXARO_INSTALL_PLUGINS]/report

INSTALLS += target

INCLUDEPATH += ../chartinterface \
  $$[EXARO_INSTALL_HEADERS]



