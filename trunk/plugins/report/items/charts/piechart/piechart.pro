TEMPLATE = lib

CONFIG += dll \
plugin \
exceptions \
  rtti \
 uitools

TARGET = PieChart

RESOURCES += piechart.qrc

HEADERS += piechart.h

SOURCES += piechart.cpp

QT += sql \
xml \
script

DESTDIR = .

INCLUDEPATH += $$[EXARO_INSTALL_HEADERS] ../abstractchartitem


macx {
    QMAKE_LFLAGS +=  -F$$[EXARO_INSTALL_LIBS]
    LIBS += -framework Report
}else {
    LIBS += -L$$[EXARO_INSTALL_LIBS] -lReport 
}

target.path = $$[EXARO_INSTALL_PLUGINS]/report

INSTALLS += target

LIBS += ../abstractchartitem/libabstractchartitem.a

TARGETDEPS += ../abstractchartitem/libabstractchartitem.a

