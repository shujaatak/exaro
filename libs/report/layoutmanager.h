/***************************************************************************
 *   Copyright (C) 2009 by Alexander Mikhalov                              *
 *   alexmi3@rambler.ru                                                    *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H

#include <QtCore>
#include <QtGui>
//#include "iteminterface.h"
//#include "bandinterface.h"

namespace Report	
{

class BandInterface;
class ItemInterface;

typedef QMap<int, BandInterface*> BandMap;
typedef QList<QGraphicsItem *> ItemList;
typedef QList<BandInterface *> BandList;

class LayoutManager /*: public QObject*/
{
public:

    static void itemAdded(ItemInterface * item);
    static void itemGeometryChanged(QObject * item);
    static void itemChangeOrder(QObject * item, int order);
    static void updatePositions(QObject * item);

    static bool splitOnLayoutTypes(QObject * item, BandMap * listTop, BandMap * listBottom, BandMap * listFree);
    static bool splitOnLayoutTypesSorted(QObject * item, BandList * listTop, BandList * listBottom, BandList * listFree);
    static BandMap sortByPriority(BandList lc);
    static BandMap sortByPriority(ItemList lc);
    static BandList sortByOrder(BandList lc);
};

}
#endif // LAYOUTMANAGER_H
