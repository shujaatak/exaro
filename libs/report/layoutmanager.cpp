#include "layoutmanager.h"

using namespace Report;

LayoutManager::LayoutManager(QObject * parent)
    :QObject(parent)
{
}

void LayoutManager::itemAdded(ItemInterface * item)
{
    qDebug("LayoutManager::itemAdded");
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
		    if (iBand->layoutPriority() > band->layoutPriority() || (iBand->layoutPriority() == band->layoutPriority() && iBand->order() < band->order()))
			by += iBand->indentation() + iBand->height();
		    else
			iBand->setPos(iBand->x(), iBand->y() + band->height() + band->indentation());
		    break;
		case BandInterface::LayoutBottom:
		    if (iBand->layoutPriority() > band->layoutPriority() || (iBand->layoutPriority() == band->layoutPriority() && iBand->order() > band->order()))
			by += iBand->indentation() + iBand->height();
		    else
			iBand->setPos(iBand->x(), iBand->y() - band->height() - band->indentation());
		    break;
	    }
    }

    if (band->layoutType() == BandInterface::LayoutTop)
	band->setPos(band->parentGeometry().x(), band->parentGeometry().y() + by);
    if (band->layoutType() == BandInterface::LayoutBottom)
	band->setPos(band->parentGeometry().x(), band->parentGeometry().y() + band->parentGeometry().height() - band->height()-  by);

    if (band->resizeFlags()|BandInterface::ResizeRight)
	band->setWidth(band->parentGeometry().width());

}


void LayoutManager::itemGeometryChanged(QObject * item)
{
    qDebug("LayoutManager::itemGeometryChanged");
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

    QMap<qreal, Report::BandInterface* > listTop;
    QMap<qreal, Report::BandInterface* > listBottom;

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
			listTop.insert((qreal)iBand->layoutPriority() + (qreal)iBand->order()/1000, iBand);
		    break;
		case BandInterface::LayoutBottom:
		    if (iBand->layoutPriority() > band->layoutPriority() || (iBand->layoutPriority() == band->layoutPriority() && iBand->order() > band->order()))
			listBottom.insert((qreal)iBand->layoutPriority() + (qreal)iBand->order()/1000, iBand);
		    break;
	    }
    }

    QList<Report::BandInterface*> list;

    if (band->layoutType() == BandInterface::LayoutTop)
    {
	list = listTop.values();
	int by =  band->geometry().bottom();
	for (int i = 0; i< list.count(); i++)
	{
	    list.at(i)->setPos(list.at(i)->x(), by + list.at(i)->indentation());
	    by += list.at(i)->height();
	}
    }

    if (band->layoutType() == BandInterface::LayoutBottom)
    {
	list = listBottom.values();
	int by =  band->geometry().top() + band->indentation();
	for (int i = 0; i< list.count(); i++)
	{
	    list.at(i)->setPos(list.at(i)->x(), by - list.at(i)->height());
	    by += list.at(i)->height() + list.at(i)->indentation();
	}
    }
}





/*
  ====================

  //    QMap<QReal, Report::BandInterface* > listTop;
//    QMap<QReal, Report::BandInterface* > listBottom;
//    QMap<QReal, Report::BandInterface* > listFree;
  //	map.insert(iBand->layoutPriority());
//	list = map.values();

*/

/*
  void BandInterface::setGeometry(QRectF rect)
{
	if (bandType() == Overlay)
	{
		ItemInterface::setGeometry(rect /+ &parentGeometry() +/);
		return;
	}

	qreal h = rect.height();
	qreal by = parentGeometry().y();

	if (bandType() == PageFooter)
		by = parentGeometry().bottom() - height();

	QList<QGraphicsItem *> lc;

	if (dynamic_cast<Report::ItemInterface*>(parentItem()))
		lc = dynamic_cast<Report::ItemInterface*>(parentItem())->childItems();
	else
		if (scene())
			lc = scene()->items();

	for (int i = 0;i < lc.size();i++)
		if (dynamic_cast<Report::BandInterface*>(lc[i]) && this != dynamic_cast<Report::BandInterface*>(lc[i]) && parent() == dynamic_cast<Report::BandInterface*>(lc[i])->parent())
			switch (bandType())
			{

				case PageHeader:
				case Title:
				case DetailContainer:
				case DetailHeader:
				case Detail:
				case DetailFooter:
				case Summary:
					if (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() >= PageHeader && dynamic_cast<Report::BandInterface*>(lc[i])->bandType() <= Summary && (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() < bandType() || (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() == bandType() && dynamic_cast<Report::BandInterface*>(lc[i])->order() < order())))
						by += dynamic_cast<Report::BandInterface*>(lc[i])->indentation() + dynamic_cast<Report::BandInterface*>(lc[i])->height();
					break;

				case PageFooter:
					if (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() >= PageFooter && dynamic_cast<Report::BandInterface*>(lc[i])->bandType() <= PageFooter && (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() > bandType() || (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() == bandType() && dynamic_cast<Report::BandInterface*>(lc[i])->order() < order())))
						by -= dynamic_cast<Report::BandInterface*>(lc[i])->indentation() + dynamic_cast<Report::BandInterface*>(lc[i])->height();
					break;
				default:
					break;
			}

	rect.setX(parentGeometry().x());

	if (by < 0)
		by = 0;

	if ((bandType() == PageFooter) && by > parentGeometry().bottom() - height())
		by = parentGeometry().bottom() - height();

	rect.setY(by);

	if (rect.y() + height() > parentGeometry().bottom())
		rect.setY(parentGeometry().bottom() - height());

	rect.setHeight(h);
	rect.setWidth(parentGeometry().width());
	ItemInterface::setGeometry(rect);
}
*/
//=================
/*
void BandInterface::setHeight(qreal height)
{
	ItemInterface::setHeight(height);

	if (!dynamic_cast<Report::ItemInterface*>(parentItem()) && !scene())
		return;

	QList<QGraphicsItem *> lc;

	if (dynamic_cast<Report::ItemInterface*>(parentItem()))
	{
		lc = dynamic_cast<Report::ItemInterface*>(parentItem())->childItems();
	}
	else
		if (scene())
			lc = scene()->items();

	for (int i = 0;i < lc.size();i++)
		if (dynamic_cast<Report::BandInterface*>(lc[i]) && !dynamic_cast<Report::BandInterface*>(lc[i])->m_deleting && this != dynamic_cast<Report::BandInterface*>(lc[i]) && parent() == dynamic_cast<Report::BandInterface*>(lc[i])->parent())
			switch (bandType())
			{
				case PageHeader:
				case Title:
				case DetailContainer:
				case DetailHeader:
				case Detail:
				case DetailFooter:
				case Summary:
					if (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() >= PageHeader && dynamic_cast<Report::BandInterface*>(lc[i])->bandType() <= Summary && (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() > bandType() || (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() == bandType() && dynamic_cast<Report::BandInterface*>(lc[i])->order() > order())))
						dynamic_cast<Report::BandInterface*>(lc[i])->updateGeometry(dynamic_cast<Report::BandInterface*>(lc[i])->geometry());
					break;

				case PageFooter:
					if (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() >= PageFooter && dynamic_cast<Report::BandInterface*>(lc[i])->bandType() <= PageFooter && (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() < bandType() || (dynamic_cast<Report::BandInterface*>(lc[i])->bandType() == bandType() && dynamic_cast<Report::BandInterface*>(lc[i])->order() > order())))
						dynamic_cast<Report::BandInterface*>(lc[i])->updateGeometry(dynamic_cast<Report::BandInterface*>(lc[i])->geometry());
					break;
				default:
					break;
			}

	if (dynamic_cast<Report::BandInterface*>(parentItem()))
	{
	    int freeSpace = dynamic_cast<Report::BandInterface*>(parentItem())->freeSpace();
	    if (freeSpace < 0)
		setHeight(geometry().height() + freeSpace);
	}
}
*/
//=================

/*
void BandInterface::setOrder(int order, bool refreshOthers)
{
	if (order < 0)
		order = 0;

	if (!refreshOthers)
	{
		m_order = order;
		return;
	}

	if (!dynamic_cast<Report::ItemInterface*>(parentItem()) && !scene())
		return;

	QList<QGraphicsItem *> lc;

	if (dynamic_cast<Report::ItemInterface*>(parentItem()))
		lc = dynamic_cast<Report::ItemInterface*>(parentItem())->childItems();
	else
		if (scene())
			lc = scene()->items();

	int max_order = 0;

	for (int i = 0;i < lc.size();i++)
		if (dynamic_cast<Report::BandInterface*>(lc[i]) && !dynamic_cast<Report::BandInterface*>(lc[i])->m_deleting && dynamic_cast<Report::BandInterface*>(lc[i])->bandType() == bandType() && this != dynamic_cast<Report::BandInterface*>(lc[i]))
		{
			max_order++;
			if (order != INT_MAX)
			{
				if (order < m_order)
				{
					if (dynamic_cast<Report::BandInterface*>(lc[i])->order() >= order && dynamic_cast<Report::BandInterface*>(lc[i])->order() < m_order)
						dynamic_cast<Report::BandInterface*>(lc[i])->setOrder(dynamic_cast<Report::BandInterface*>(lc[i])->order() + 1, false);
				}
				else
					if (dynamic_cast<Report::BandInterface*>(lc[i])->order() <= order && dynamic_cast<Report::BandInterface*>(lc[i])->order() > m_order)
						dynamic_cast<Report::BandInterface*>(lc[i])->setOrder(dynamic_cast<Report::BandInterface*>(lc[i])->order() - 1, false);
			}
		}

	if (order > max_order)
		order = max_order;

	m_order = order;

	for (int i = 0;i < lc.size();i++)
		if (dynamic_cast<Report::BandInterface*>(lc[i]) && !dynamic_cast<Report::BandInterface*>(lc[i])->m_deleting && this != dynamic_cast<Report::BandInterface*>(lc[i]) && dynamic_cast<Report::BandInterface*>(lc[i])->bandType() != BandInterface::Overlay )
			dynamic_cast<Report::BandInterface*>(lc[i])->updateGeometry(dynamic_cast<Report::BandInterface*>(lc[i])->geometry());

	updateGeometry(geometry());
}

*/
