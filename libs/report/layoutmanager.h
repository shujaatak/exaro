#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H

#include <QObject>
#include "pageinterface.h"
#include "iteminterface.h"
#include "bandinterface.h"
namespace Report
{

class LayoutManager : public QObject
{
public:
    LayoutManager(QObject * parent = 0);

    static void itemAdded(ItemInterface * item);
    static void itemGeometryChanged(QObject * item);
};

}
#endif // LAYOUTMANAGER_H
