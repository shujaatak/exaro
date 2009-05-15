#!/bin/bash

CMAKE_INSTALL_PREFIX=$1
shift;
PROJECT_SOURCE_DIR=$1
shift;

mkdir ${CMAKE_INSTALL_PREFIX}/bin/eXaro.app/Contents/PlugIns
cp -pr ${CMAKE_INSTALL_PREFIX}/lib/propertyEditor ${CMAKE_INSTALL_PREFIX}/bin/eXaro.app/Contents/PlugIns
cp -pr ${CMAKE_INSTALL_PREFIX}/lib/report ${CMAKE_INSTALL_PREFIX}/bin/eXaro.app/Contents/PlugIns
cp -pr ${CMAKE_INSTALL_PREFIX}/lib/eXaro ${CMAKE_INSTALL_PREFIX}/bin/eXaro.app/Contents/PlugIns

mkdir ${CMAKE_INSTALL_PREFIX}/bin/eXaro.app/Contents/Frameworks
/usr/bin/perl ${PROJECT_SOURCE_DIR}/util/pkgApp.pl ${CMAKE_INSTALL_PREFIX}/bin eXaro.app/Contents/MacOS/eXaro eXaro.app/Contents/Frameworks 
#cp -pr /Developer/Applications/Qt/plugins/{imageformats,iconengines} ${CMAKE_INSTALL_PREFIX}/MacOS


for f in ${CMAKE_INSTALL_PREFIX}/bin/eXaro.app/Contents/PlugIns/{propertyEditor,report}/*.dylib; do
#	/usr/bin/perl ${PROJECT_SOURCE_DIR}/util/pkgApp.pl ${CMAKE_INSTALL_PREFIX}/bin ${CMAKE_INSTALL_PREFIX} $f eXaro.app/Contents/Frameworks
    fdir=`dirname $f`
    dir=`basename $fdir`
    base=`basename $f`
/usr/bin/perl ${PROJECT_SOURCE_DIR}/util/pkgApp.pl ${CMAKE_INSTALL_PREFIX}/bin eXaro.app/Contents/PlugIns/$dir/$base eXaro.app/Contents/Frameworks 
done


for f in ${CMAKE_INSTALL_PREFIX}/bin/eXaro.app/Contents/PlugIns/report/export/*.dylib; do
#	/usr/bin/perl ${PROJECT_SOURCE_DIR}/util/pkgApp.pl ${CMAKE_INSTALL_PREFIX}/bin ${CMAKE_INSTALL_PREFIX} $f eXaro.app/Contents/Frameworks
    fdir=`dirname $f`
    dir=`basename $fdir`
    base=`basename $f`
/usr/bin/perl ${PROJECT_SOURCE_DIR}/util/pkgApp.pl ${CMAKE_INSTALL_PREFIX}/bin eXaro.app/Contents/PlugIns/report/$dir/$base eXaro.app/Contents/Frameworks 
done

for f in ${CMAKE_INSTALL_PREFIX}/bin/eXaro.app/Contents/PlugIns/eXaro/toolbars/*.dylib; do
#	/usr/bin/perl ${PROJECT_SOURCE_DIR}/util/pkgApp.pl ${CMAKE_INSTALL_PREFIX}/bin ${CMAKE_INSTALL_PREFIX} $f eXaro.app/Contents/Frameworks
    fdir=`dirname $f`
    dir=`basename $fdir`
    base=`basename $f`
/usr/bin/perl ${PROJECT_SOURCE_DIR}/util/pkgApp.pl ${CMAKE_INSTALL_PREFIX}/bin eXaro.app/Contents/PlugIns/eXaro/$dir/$base eXaro.app/Contents/Frameworks 
done

