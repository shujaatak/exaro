TEMPLATE = lib

CONFIG += staticlib \
exceptions \
  rtti \
 uitools

QT += sql \
xml \
script

INCLUDEPATH += $$[EXARO_INSTALL_HEADERS] ../chartcontainer

macx {
    QMAKE_LFLAGS +=  -F$$[EXARO_INSTALL_LIBS]
    LIBS += -framework Report
}else {
    LIBS += -L$$[EXARO_INSTALL_LIBS] -lReport
}

HEADERS += chartinterface.h \
 chartdatacollector.h

SOURCES += chartinterface.cpp \
 chartdatacollector.cpp

