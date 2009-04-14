#!/bin/sh

lrelease translations.pro
mkdir -p /usr/local/share/eXaro/translations
cp *.qm /usr/local/share/eXaro/translations
