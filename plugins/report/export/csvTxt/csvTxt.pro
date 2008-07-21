TEMPLATE = lib

CONFIG += dll \
plugin \
exceptions \
  rtti \
 uitools

TARGET = CsvTxt

QT += sql \
xml \
script

HEADERS += csvtxt.h \
exportsettingsdialog.h

SOURCES += csvtxt.cpp \
exportsettingsdialog.cpp

FORMS += exportSettingsDialog.ui

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

