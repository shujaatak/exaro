#!/bin/sh

lrelease translations.pro
mkdir -p `qmake -query EXARO_INSTALL_TRANSLATIONS`
cp *.qm `qmake -query EXARO_INSTALL_TRANSLATIONS`
