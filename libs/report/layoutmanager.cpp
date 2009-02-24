#include "QtCore"
#include "layoutmanager.h"

using namespace Report;

LayoutManager::LayoutManager(QObject * parent)
    :QObject(parent)
{
}

void LayoutManager::itemAdded(ItemInterface * item)
{
    Q_ASSERT(item);

    if (!dynamic_cast<Report::BandInterface*>(item))
	return;

    BandInterface * band = dynamic_cast<Report::BandInterface*>(item);

    if (band->layoutType() == BandInterface::LayoutFree)
	return;

    QList<QGraphicsItem *> lc;    
    if (dynamic_cast<Report::ItemInterface*>(band->parentItem()))
	lc = dynamic_cast<Report::ItemInterface*>(band->parentItem())->childItems();
    else
	if (band->scene())
	    lc = band->scene()->items();

    qreal by;
    int maxOrder = -1;

    for (int i = 0;i < lc.size();i++)
    {
	Report::BandInterface * iBand;
	if (dynamic_cast<Report::BandInterface*>(lc[i]))
	    iBand = dynamic_cast<Report::BandInterface*>(lc[i]);
	else
	    continue;

	if (band != iBand && band->parent() == iBand->parent() && band->layoutType() == iBand->layoutType())
	    switch (band->layoutType())
	    {
		case BandInterface::LayoutTop:
		    if (iBand->layoutPriority() >= band->layoutPriority() /*|| (iBand->layoutPriority() == band->layoutPriority() && iBand->order() < band->order())*/)
			by += iBand->indentation() + iBand->height();
		    else
			iBand->setPos(iBand->x(), iBand->y() + band->height() + band->indentation());
		    if (iBand->layoutPriority() == band->layoutPriority() && iBand->order() > maxOrder)
			maxOrder = iBand->order();
		    break;
		case BandInterface::LayoutBottom:
		    if (iBand->layoutPriority() >= band->layoutPriority() /*|| (iBand->layoutPriority() == band->layoutPriority() && iBand->order() > band->order())*/)
			by += iBand->indentation() + iBand->height();
		    else
			iBand->setPos(iBand->x(), iBand->y() - band->height() - band->indentation());
		    if (iBand->layoutPriority() == band->layoutPriority())
			iBand->setOrder(iBand->order() + 1, false);
		    break;
	    }
    }

    if (band->layoutType() == BandInterface::LayoutTop)
    {
	band->setPos(band->parentGeometry().x(), band->parentGeometry().y() + by);
	band->setOrder(maxOrder + 1, false);
    }
    if (band->layoutType() == BandInterface::LayoutBottom)
    {
	band->setPos(band->parentGeometry().x(), band->parentGeometry().y() + band->parentGeometry().height() - band->height()-  by);
	band->setOrder(0, false);
    }

    if (band->resizeFlags()|BandInterface::ResizeRight)
	band->setWidth(band->parentGeometry().width());

}


void LayoutManager::itemGeometryChanged(QObject * item)
{
    Q_ASSERT(item);

    if (!dynamic_cast<Report::BandInterface*>(item))
	return;

    BandInterface * band = dynamic_cast<Report::BandInterface*>(item);

    if (band->layoutType() == BandInterface::LayoutFree)
	return;

    QList<QGraphicsItem *> lc;
    if (dynamic_cast<Report::ItemInterface*>(band->parentItem()))
	lc = dynamic_cast<Report::ItemInterface*>(band->parentItem())->childItems();
    else
	if (band->scene())
	    lc = band->scene()->items();

    QMap<qreal, Report::BandInterface*> listTop;
    QMap<qreal, Report::BandInterface*> listBottom;

    for (int i = 0;i < lc.size();i++)
    {
	Report::BandInterface * iBand;
	if (dynamic_cast<Report::BandInterface*>(lc[i]))
	    iBand = dynamic_cast<Report::BandInterface*>(lc[i]);
	else
	    continue;

	if (band != iBand && band->parent() == iBand->parent() && band->layoutType() == iBand->layoutType())
	    switch (band->layoutType())
	    {
		case BandInterface::LayoutTop:
		    if (iBand->layoutPriority() < band->layoutPriority() || (iBand->layoutPriority() == band->layoutPriority() && iBand->order() > band->order()))
			listTop.insert((qreal)iBand->layoutPriority() + (qreal)(INT_MAX - iBand->order())/INT_MAX , iBand);
		    break;
		case BandInterface::LayoutBottom:
		    if (iBand->layoutPriority() < band->layoutPriority() || (iBand->layoutPriority() == band->layoutPriority() && iBand->order() < band->order()))
			listBottom.insert((qreal)iBand->layoutPriority() + (qreal)iBand->order()/INT_MAX, iBand);
		    break;
	    }
    }

    QList<Report::BandInterface*> list;

    if (band->layoutType() == BandInterface::LayoutTop)
    {
	list = listTop.values();
	int by =  band->geometry().bottom();
	for (int i = list.count()-1; i>=0 ;i--)
	{
	    list.at(i)->setPos(list.at(i)->x(), by + list.at(i)->indentation());
	    by += list.at(i)->height();
	}
    }

    if (band->layoutType() == BandInterface::LayoutBottom)
    {
	list = listBottom.values();
	int by =  band->geometry().top() + band->indentation();
	for (int i = list.count()-1; i>=0 ;i--)
	{
	    list.at(i)->setPos(list.at(i)->x(), by - list.at(i)->height());
	    by += list.at(i)->height() + list.at(i)->indentation();
	}
    }
}


void LayoutManager::itemChangeOrder(QObject * item, int order)
{
    qDebug("LayoutManager::itemChangeOrder");
    BandInterface * band = dynamic_cast<Report::BandInterface*>(item);
    Q_ASSERT(band);

    if (!band->parentItem() && !band->scene())
	return;
    if (band->layoutType() == BandInterface::LayoutFree)
    {
	band->setOrder(order, false);
	return;
    }


    QList<QGraphicsItem *> lc;

    if (dynamic_cast<Report::ItemInterface*>(band->parentItem()))
	lc = dynamic_cast<Report::ItemInterface*>(band->parentItem())->childItems();
    else
	if (band->scene())
	    lc = band->scene()->items();

    int max_order = 0;
//    QMap<int, Report::BandInterface* > list;

    for (int i = 0;i < lc.size();i++)
    {
	Report::BandInterface* iBand = dynamic_cast<Report::BandInterface*>(lc[i]);
	if (iBand && !iBand->deleting() && iBand->layoutType() == band->layoutType() && iBand->layoutPriority() == band->layoutPriority() && band != iBand)
	{
	    max_order++;
	    if (order != INT_MAX)
	    {
		if (order < band->order())
		{
		    if (iBand->order() >= order && iBand->order() < band->order())
//			list.insert(iBand->order() + 1, iBand);
			iBand->setOrder(iBand->order() + 1, false);
		}
		else
		    if (iBand->order() <= order && iBand->order() > band->order())
//			list.insert(iBand->order() - 1, iBand);
			iBand->setOrder(iBand->order() - 1, false);
	    }
	}
    }
    if (order > max_order)
	order = max_order;

    band->setOrder(order, false);

    if (dynamic_cast<Report::ItemInterface*>(band->parentItem()))
	updatePositions(dynamic_cast<Report::ItemInterface*>(band->parentItem()));
    else
	updatePositions(band->scene());
}

void LayoutManager::updatePositions(QObject * item)
{
    qDebug("LayoutManager::updatePositions");
    Q_ASSERT(item);

    BandMap listTop;
    BandMap listBottom;

    if (!splitOnLayoutTypes(item, &listTop, &listBottom))
	return;

    QRectF rect;
    if (dynamic_cast<Report::ItemInterface*>(item))
	rect = dynamic_cast<Report::ItemInterface*>(item)->geometry();
    else
	if (dynamic_cast<Report::PageInterface*>(item))
	    rect = dynamic_cast<Report::PageInterface*>(item)->geometry();

    QList<int> pList;
    int by;

    by = rect.top();
    pList = listTop.uniqueKeys();
    for (int i = pList.count()-1; i>=0 ;i--)
    {
	qDebug("i=%i",i);
	BandList orderList = sortByOrder(listTop.values(pList.at(i)));
	for (int j = 0; j<orderList.count() ;j++)
	{
	    qDebug("j=%i, item=%s",j, qPrintable(orderList.at(j)->objectName()));
	    orderList.at(j)->setPos(orderList.at(j)->x(), by + orderList.at(j)->indentation());
	    by += orderList.at(j)->height();
	}
    }

    /*
    list = listTop.values();
    qDebug("top count = %i", list.count());
    //by = dynamic_cast<Report::BandInterface*> (item)->geometry().bottom();
    by = rect.top();
    for (int i = list.count()-1; i>=0 ;i--)
    {
	qDebug("i=%i, item=%s",i, qPrintable(list.at(i)->objectName()));
	list.at(i)->setPos(list.at(i)->x(), by + list.at(i)->indentation());
	by += list.at(i)->height();
    }
    */
    ///set Bottom items
    /*
    list = listBottom.values();
    qDebug("bottom count = %i", list.count());
    //by = dynamic_cast<Report::BandInterface*> (item)->geometry().top() + dynamic_cast<Report::BandInterface*> (item)->indentation();
    by = rect.bottom();
    for (int i = list.count()-1; i>=0 ;i--)
    {
	qDebug("i=%i, item=%s",i, qPrintable(list.at(i)->objectName()));
	list.at(i)->setPos(list.at(i)->x(), by - list.at(i)->height());
	by += list.at(i)->height() + list.at(i)->indentation();
    }
    */
}


bool LayoutManager::splitOnLayoutTypes(QObject * item, BandMap * listTop, BandMap * listBottom )
{
    QList<QGraphicsItem *> lc;
    if (dynamic_cast<Report::ItemInterface*>(item))
	lc = dynamic_cast<Report::ItemInterface*>(item)->childItems();
    else
	if (dynamic_cast<Report::PageInterface*>(item))
	    lc = dynamic_cast<Report::PageInterface*>(item)->items();

    if (lc.isEmpty())
	return false;

    for (int i = 0;i < lc.size();i++)
    {
	Report::BandInterface * iBand;
	if (dynamic_cast<Report::BandInterface*>(lc[i]))
	    iBand = dynamic_cast<Report::BandInterface*>(lc[i]);
	else
	    continue;

	if (item == iBand->parent())
	    switch (iBand->layoutType())
	    {
		case BandInterface::LayoutTop:
		    listTop->insertMulti(iBand->layoutPriority(), iBand);
		    break;
		case BandInterface::LayoutBottom:
		    listBottom->insertMulti(iBand->layoutPriority(), iBand);
		    break;
	    }
    }
    return true;
}


BandMap LayoutManager::sortByPriority(ItemList lc)
{
    BandMap nList;
    for (int i = 0;i < lc.size();i++)
    {
	Report::BandInterface * iBand;

	if (dynamic_cast<Report::BandInterface*>(lc[i]))
	    iBand = dynamic_cast<Report::BandInterface*>(lc[i]);
	else
	    continue;
	nList.insert(iBand->layoutPriority(), iBand);
    }
    return nList;
}

BandMap LayoutManager::sortByPriority(BandList lc)
{
    BandMap nList;
    for (int i = 0;i < lc.size();i++)
	nList.insert(lc[i]->layoutPriority(), lc[i]);
    return nList;
}

BandList LayoutManager::sortByOrder(BandList lc)
{
    BandMap nList;
    for (int i = 0;i < lc.size();i++)
	nList.insert(lc[i]->order(), lc[i]);
    return nList.values();
}
