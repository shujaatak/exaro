TEMPLATE = lib

CONFIG += qt warn_on \
 dll

macx{
	CONFIG -= dll
	CONFIG += lib_bundle
}

	
QT -= gui

LIBS += -lz

# Input
HEADERS += crypt.h \
           ioapi.h \
           quazip.h \
           quazipfile.h \
           quazipfileinfo.h \
           quazipnewinfo.h \
           unzip.h \
           zip.h

SOURCES += ioapi.c \
           quazip.cpp \
           quazipfile.cpp \
           quazipnewinfo.cpp \
           unzip.c \
           zip.c

headers.path=$$[QUAZIP_INSTALL_HEADERS]/quazip
headers.files=$$HEADERS
target.path=$$[QUAZIP_INSTALL_LIBS]
INSTALLS += headers target
