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

#include "QtCore"
#include "layoutmanager.h"
#include "pageinterface.h"
#include "iteminterface.h"
#include "bandinterface.h"

using namespace Report;

#define MARGIN 50

void LayoutManager::itemAdded(ItemInterface * item)
{
    Q_ASSERT(item);

    qDebug("LayoutManager::itemAdded  name = %s", qPrintable(item->objectName()));

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
			by += /*iBand->indentation() +*/ iBand->height() + iBand->title()->boundingRect().height() + (i?MARGIN:0);
		    else
			iBand->setPos(iBand->x(), iBand->y() + band->height() + band->title()->boundingRect().height() + (i?MARGIN:0)/*+ band->indentation()*/);
		    if (iBand->layoutPriority() == band->layoutPriority() && iBand->order() > maxOrder)
			maxOrder = iBand->order();
		    break;
		case BandInterface::LayoutBottom:
		    if (iBand->layoutPriority() >= band->layoutPriority() /*|| (iBand->layoutPriority() == band->layoutPriority() && iBand->order() > band->order())*/)
			by += /*iBand->indentation() +*/ iBand->height() + iBand->title()->boundingRect().height() + (i?MARGIN:0);
		    else
			iBand->setPos(iBand->x(), iBand->y() - band->height() - band->title()->boundingRect().height() - (i?MARGIN:0)/*- band->indentation()*/);
		    if (iBand->layoutPriority() == band->layoutPriority())
			iBand->setOrder(iBand->order() + 1, false);
		    break;
	    }
    }

    if (band->layoutType() == BandInterface::LayoutTop)
    {
	band->setPos(band->parentGeometry().x(), band->parentGeometry().y() + by + band->title()->boundingRect().height());
	band->setOrder(maxOrder + 1, false);
    }
    if (band->layoutType() == BandInterface::LayoutBottom)
    {
	band->setPos(band->parentGeometry().x(), band->parentGeometry().y() + band->parentGeometry().height() - band->height()-  by);
	band->setOrder(0, false);
    }

    if (band->resizeFlags()|BandInterface::ResizeRight)
	band->setWidth(band->parentGeometry().width());

    qDebug("order is %i,  by=%f", band->order(), by);
}


void LayoutManager::ItemDelete(ItemInterface * item, QObject * parent)
{
    Q_ASSERT(item);

    qDebug("LayoutManager::ItemDelete  name = %s, parent name ", qPrintable(item->objectName()), qPrintable(parent->objectName()));

    if (!dynamic_cast<Report::BandInterface*>(item))
	return;

    BandInterface * band = dynamic_cast<Report::BandInterface*>(item);

    if (band->layoutType() == BandInterface::LayoutFree)
	return;

    QList<QGraphicsItem *> lc;
    if (dynamic_cast<Report::ItemInterface*>(parent))
	lc = dynamic_cast<Report::ItemInterface*>(band->parentItem())->childItems();
    else
	if (dynamic_cast <Report::PageInterface*> (parent))
	    lc = dynamic_cast <Report::PageInterface*> (parent)->items();
	    else
		qCritical("developer error in Layout Manager ItemDelete");

    int priority = band->layoutPriority();
    BandInterface::LayoutType lType = band->layoutType();
    int order = band->order();
    qDebug("number items = %i", lc.count());

    foreach(QGraphicsItem * tItem, lc)
	if (dynamic_cast<Report::BandInterface*>(tItem))
	{
	    BandInterface * iBand = dynamic_cast<BandInterface*>(tItem);
	    if (iBand->layoutType() == lType && iBand->layoutPriority() == priority && iBand->order() >= order)
		iBand->setOrder(iBand->order() - 1, false);
	}

    //band->setOrder(INT_MAX, false);

    updatePositions(parent);
}



void LayoutManager::itemGeometryChanged(QObject * item)
{
    Q_ASSERT(item);

    qDebug("LayoutManager::itemGeometryChanged name = %s", qPrintable(item->objectName()));

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
	    list.at(i)->setPos(list.at(i)->x(), by + list.at(i)->title()->boundingRect().height() + MARGIN/*+ list.at(i)->indentation()*/);
	    by += list.at(i)->height() + list.at(i)->title()->boundingRect().height() + MARGIN;
	}
    }

    if (band->layoutType() == BandInterface::LayoutBottom)
    {
	list = listBottom.values();
	int by =  band->geometry().top() /*+ band->indentation()*/;
	for (int i = list.count()-1; i>=0 ;i--)
	{
	    list.at(i)->setPos(list.at(i)->x(), by - list.at(i)->height());
	    by += list.at(i)->height() + list.at(i)->title()->boundingRect().height() + MARGIN/*+ list.at(i)->indentation()*/;
	}
    }
}


void LayoutManager::itemChangeOrder(QObject * item, int order)
{
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
			iBand->setOrder(iBand->order() + 1, false);
		}
		else
		    if (iBand->order() <= order && iBand->order() > band->order() && band->order() >= 0) //band->order() == -1 for uninitialised band
			iBand->setOrder(iBand->order() - 1, false);
	    }
	}
    }

    if (order < 0)
	order = 0;
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
    Q_ASSERT(item);


    BandMap listTop;
    BandMap listBottom;
    BandMap listFree;

    if (!LayoutManager::splitOnLayoutTypes(item, &listTop, &listBottom, &listFree))
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
	BandList orderList = sortByOrder(listTop.values(pList.at(i)));
	for (int j = 0; j<orderList.count() ;j++)
	{
	    orderList.at(j)->setPos(orderList.at(j)->x(), by + orderList.at(j)->title()->boundingRect().height() + (by==rect.top()?0:MARGIN)/*+ orderList.at(j)->indentation()*/);
	    by += orderList.at(j)->height() + orderList.at(j)->title()->boundingRect().height() + (by==rect.top()?0:MARGIN);
	}
    }

    by = rect.bottom();
    pList = listBottom.uniqueKeys();
    for (int i = pList.count()-1; i>=0 ;i--)
    {
	BandList orderList = sortByOrder(listBottom.values(pList.at(i)));
	for (int j = orderList.count() -1; j>=0 ;j--)
	{
	    orderList.at(j)->setPos(orderList.at(j)->x(), by - orderList.at(j)->height());
	    by -= orderList.at(j)->height() + orderList.at(j)->title()->boundingRect().height() + MARGIN/*- orderList.at(j)->indentation()*/;
	}
    }
}


bool LayoutManager::splitOnLayoutTypes(QObject * item, BandMap * listTop, BandMap * listBottom, BandMap * listFree)
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
		case BandInterface::LayoutFree:
		    listFree->insertMulti(iBand->layoutPriority(), iBand);
		    break;
	    }
    }
    return true;
}

bool LayoutManager::splitOnLayoutTypesSorted(QObject * item, BandList * listTop, BandList * listBottom, BandList * listFree)
{
    BandMap m_listTop;
    BandMap m_listBottom;
    BandMap m_listFree;

    splitOnLayoutTypes(item, &m_listTop, &m_listBottom, &m_listFree);

    QList<int> pList;

    pList = m_listTop.uniqueKeys();
    for (int i = pList.count()-1; i>=0 ;i--)
    {
	BandList orderList = sortByOrder(m_listTop.values(pList.at(i)));
	for (int j = 0; j<orderList.count() ;j++)
	    listTop->append(orderList.at(j));
    }

    pList = m_listBottom.uniqueKeys();
    for (int i = pList.count()-1; i>=0 ;i--)
    {
	BandList orderList = sortByOrder(m_listBottom.values(pList.at(i)));
	for (int j = 0; j<orderList.count() ;j++)
	    listBottom->append(orderList.at(j));
    }

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
	nList.insertMulti(lc[i]->order(), lc[i]);
    return nList.values();
}
