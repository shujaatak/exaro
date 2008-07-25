TEMPLATE =  app

CONFIG +=  exceptions \
  qt \
  rtti \
  uitools

QT +=   core \
  gui \
  sql \
  svg \
  xml \
  network \
  script

TARGET =   exaro

SOURCES +=   main.cpp \
  mainwindow.cpp \
  designerquerywidget.cpp \
  designerpage.cpp \
   queryeditdialog.cpp \
   sqlsyntaxhighlighter.cpp \
   scriptsyntaxhighlighter.cpp \
   scripteditdialog.cpp \
   designeruiwidget.cpp \
   designeruitype.cpp \
   sqldatabasedialog.cpp \
   objectmodel.cpp \
   aboutdialog.cpp \
   exaroapplication.cpp

FORMS +=   mainwindow.ui \
  designerquerywidget.ui \
   designeruiwidget.ui \
   designeruitype.ui \
   sqldatabasedialog.ui \
   aboutdialog.ui

HEADERS +=   mainwindow.h \
  designerquerywidget.h \
  designerpage.h \
   queryeditdialog.h \
   sqlsyntaxhighlighter.h \
   scriptsyntaxhighlighter.h \
   scripteditdialog.h \
   designeruiwidget.h \
   designeruitype.h \
   sqldatabasedialog.h \
   objectmodel.h \
   aboutdialog.h \
   exaroapplication.h

RESOURCES +=   report.qrc


DESTDIR = .

INCLUDEPATH += $$[EXARO_INSTALL_HEADERS]


macx {
    QMAKE_LFLAGS +=  -F$$[EXARO_INSTALL_LIBS]
    LIBS += -framework Report -framework PropertyEditor
}else {
    LIBS += -L$$[EXARO_INSTALL_LIBS] -lReport -lPropertyEditor
}


target.path = $$[EXARO_INSTALL_BINS]

INSTALLS += target

win32 {
    RC_FILE = exaro.rc

    icon.files = exaro.ico exaro.nsi
    icon.path = $$[EXARO_INSTALL_BINS]
    INSTALLS += icon
}
unix {
    !macx {
        setup.commands = update-mime-database $$[EXARO_INSTALL_MIMES]
        QMAKE_EXTRA_UNIX_TARGETS += setup
        icon.files = application-bdrt.png
        icon.path = $$[EXARO_INSTALL_ICONS]
        menu.files = exaro.desktop
        menu.path = $$[EXARO_INSTALL_MENUS]
        mimelnk.files = bdrt.desktop
        mimelnk.path = $$[EXARO_INSTALL_MIMELNKS]
        mimelnk.depends = setup
        mime.files = bdrt.xml
        mime.path = $$[EXARO_INSTALL_MIMES]/packages
        INSTALLS += icon menu mime mimelnk 
    }
}
