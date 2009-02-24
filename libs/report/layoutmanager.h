#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H

#include <QObject>
#include "pageinterface.h"
#include "iteminterface.h"
#include "bandinterface.h"

namespace Report	
{
typedef QMap<int, Report::BandInterface*> BandMap;
typedef QList<QGraphicsItem *> ItemList;
typedef QList<BandInterface *> BandList;

class LayoutManager : public QObject
{
public:
    LayoutManager(QObject * parent = 0);

    static void itemAdded(ItemInterface * item);
    static void itemGeometryChanged(QObject * item);
    static void itemChangeOrder(QObject * item, int order);
    static void updatePositions(QObject * item);

//    static QMap<int,BandInterface*> sortByPriority(QList<QGraphicsItem *> lc);
    static bool splitOnLayoutTypes(QObject * item, BandMap * listTop, BandMap * listBottom);
    static BandMap sortByPriority(BandList lc);
    static BandMap sortByPriority(ItemList lc);
    static BandList sortByOrder(BandList lc);
};

}
#endif // LAYOUTMANAGER_H
