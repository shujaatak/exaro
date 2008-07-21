qmake -set ZINT_INSTALL_LIBS d:\BogDan\bin\eXaro
qmake -set ZINT_INSTALL_HEADERS d:\BogDan\bin\eXaro\include
qmake -set QUAZIP_INSTALL_LIBS d:\BogDan\bin\eXaro
qmake -set QUAZIP_INSTALL_HEADERS d:\BogDan\bin\eXaro\include
qmake -set EXARO_INSTALL_BINS d:\BogDan\bin\eXaro
qmake -set EXARO_INSTALL_LIBS d:\BogDan\bin\eXaro
qmake -set EXARO_INSTALL_HEADERS d:\BogDan\bin\eXaro\include
qmake -set EXARO_INSTALL_PLUGINS d:\BogDan\bin\eXaro
qmake -set EXARO_INSTALL_TRANSLATIONS d:\BogDan\bin\eXaro\translations

cd libs && qmake  && make -j2 && make install && cd ..\plugins && qmake && make -j2 && make install && cd ..\apps && qmake && make -j2 && make install && cd ..
