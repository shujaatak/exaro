#include "options.h"

QSize Options::iconSize()
{
   QString iSize = s.value("Options/iconSize").toString();
   int h,w;
   h = iSize.section("x",0,0).toInt();
   w = iSize.section("x",1,1).toInt();
   if (h=0 || w=0)
       return QSize(32,32)
   else
       return QSize(w,h);
}
