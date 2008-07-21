TEMPLATE = lib

CONFIG += dll \
plugin \
exceptions \
  rtti \
 uitools


TARGET = ChartContainer

RESOURCES += chartcontainer.qrc

HEADERS += chartcontainer.h

SOURCES += chartcontainer.cpp

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

target.path = $$[EXARO_INSTALL_PLUGINS]/report

INSTALLS += target

INCLUDEPATH += ../chartinterface \
  $$[EXARO_INSTALL_HEADERS]

LIBS += ../chartinterface/libchartinterface.a

TARGETDEPS += ../chartinterface/libchartinterface.a

