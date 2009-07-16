/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        * 
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      * 
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#include <QGraphicsScene>
#include <QPainter>

#include "bandinterface.h"
#include "pageinterface.h"
#include "layoutmanager.h"

#include <limits.h>

namespace Report
{

BandInterface::BandInterface(QGraphicsItem* parent, QObject * parentObject)
		: ItemInterface(parent, parentObject),
		m_deleting(false),
		m_titleItem(0)
{
	setResizeFlags(FixedPos | ResizeTop | ResizeBottom);

	if (!parentObject)
		return;

	if (dynamic_cast<ItemInterface*>(parent))
		connect(dynamic_cast<ItemInterface*>(parent), SIGNAL(geometryChanged(QRectF)), this, SLOT(updateGeometry(QRectF)));
	else
		connect(parentObject, SIGNAL(geometryChanged(QRectF)), this, SLOT(updateGeometry(QRectF)));

	m_order = -1;
	m_indentationTop = m_indentationBottom = 0;
	m_dataset = "";

	setHeight(20/UNIT);
}

void BandInterface::selectBand()
{
	emit(itemSelected(this, QPointF(0,0)));
	raise();
}

bool BandInterface::deleting()
{
	return m_deleting;
}

BandInterface::~BandInterface()
{
    delete m_titleItem;
}

int BandInterface::freeSpace()
{
    QList<QGraphicsItem *> lc = childItems();
    int mSize = 0;

    foreach (QGraphicsItem * item, lc)
	if (dynamic_cast<Report::BandInterface*>(item))
	    mSize += dynamic_cast<Report::BandInterface*>(item)->height();
    return height() - mSize;
}

void BandInterface::setIndentationTop(int indentation)
{
	m_indentationTop = indentation;
	setHeight(height());
}

void BandInterface::setIndentationBottom(int indentation)
{
	m_indentationBottom = indentation;
	setHeight(height());
}

int BandInterface::indentationTop()
{
	return m_indentationTop;
}

int BandInterface::indentationBottom()
{
	return m_indentationBottom;
}

void BandInterface::setOrder(int order, bool refreshOthers)
{

	if (order < 0)
		order = 0;

	if (!refreshOthers)
	{
		m_order = order;
		return;
	}

	LayoutManager::itemChangeOrder(this, order);

}

int BandInterface::order()
{
	return m_order;
}


void BandInterface::setHeight(qreal height)
{
	ItemInterface::setHeight(height);
	LayoutManager::itemGeometryChanged(this);
}

void BandInterface::updateGeometry(QRectF rect)
{
	prepareGeometryChange();
	setGeometry(QRectF(parentGeometry().x(), geometry().y(), rect.width(), height()));
}

void BandInterface::createTitle(const QString & title)
{
    if (!m_titleItem)
    {
	m_titleItem=new BandTitle(this, QSizeF(geometry().width(), 40), title);
	scene()->addItem(m_titleItem);
	connect (this, SIGNAL(geometryChanged(QRectF)), this, SLOT(setTitleGeometry(QRectF)) );
    }
}

void BandInterface::setTitleGeometry(QRectF rect)
{
    m_titleItem->setSize(QSize(rect.width(), m_titleItem->boundingRect().height() ) );
}

/*
void BandInterface::drawTitle(const QString & title, TitlePosition position, int textFlags)
{
	QPointF pos;
	if(position==TitleLeft)
	{
		if (parentItem())
		{
			pos.setX(parentItem()->pos().x()+geometry().left()-40);
			pos.setY(parentItem()->pos().y()+geometry().top());
		}
		else
		{
			pos.setX(geometry().left()-40);
			pos.setY(geometry().top());
		}
	}
	else
	{
		if (parentItem())
		{
			pos.setX(parentItem()->pos().x()+geometry().right());
			pos.setY(parentItem()->pos().y()+geometry().top());
		}
		else
		{
			pos.setX(geometry().right());
			pos.setY(geometry().top());
		}
	}
	if (!m_titleItem)
	{
		m_titleItem=new TitleItem(this, QSizeF(40,geometry().height()), title, textFlags, position);
		scene()->addItem(m_titleItem);
		m_titleItem->setPos(pos);
	}
	else
	{
		if (m_titleItem->size().height()!=geometry().height() || m_titleItem->pos()!=pos)
		{
			m_titleItem->setSize(QSizeF(40,geometry().height()));
			m_titleItem->setPos(pos);
		}
	}
}
*/

void BandInterface::setGeometry(QRectF rect)
{
	ItemInterface::setGeometry(rect);
	// be layout in ItemInterface setHeight
	//Report::LayoutManager::itemGeometryChanged(this);
}


BandInterface::LayoutType BandInterface::layoutType()
{
	return LayoutTop;
}

int BandInterface::layoutPriority()
{
    return 0;
}

BandInterface::AccomodationType BandInterface::accommodationType()
{
    return AccomodationOnce;
}

bool BandInterface::prNewPage() {return false;}
bool BandInterface::prClosePage() {return false;}


QString BandInterface::dataset()
{
	return m_dataset;
}

void BandInterface::setDataset(const QString & dataset)
{
	m_dataset = dataset;
}

void BandInterface::addAgregateValue(QString value)
{
    if (!m_agregateValues.contains(value))
    {
	ValueStruct vStruct;
	if (stringIsField(value))
	    stringToField (value, &vStruct.dataset, &vStruct.field);
	m_agregateValues.insert(value, vStruct);
    }
}

QList<qreal> BandInterface::agregateValues(QString value)
{
//   qDebug("BandInterface::agregateValues contain value \"%s\" - %i", qPrintable(value), (int)m_agregateValues.contains(value));
    if (m_agregateValues.contains(value))
	return m_agregateValues.value(value).list;
    else
	return QList<qreal>();
}

void BandInterface::accumulateAgregateValues()
{
    QMutableHashIterator <QString, ValueStruct> i(m_agregateValues);
    while (i.hasNext()) {
	i.next();
	QString value = i.key();
	ValueStruct * vStruct = &i.value();
	if (vStruct->dataset.isEmpty())
	    vStruct->list.append( scriptEngine()->globalObject().property(value).toNumber() );
	else
	    vStruct->list.append( datasetField(vStruct->dataset, vStruct->field).toDouble() );

    }
    m_agregateCounter++;
}

void BandInterface::resetAgregateValues()
{
    QMutableHashIterator <QString, ValueStruct> i(m_agregateValues);
    while (i.hasNext()) {
	i.next();
	i.value().list.clear();
    }
    m_agregateCounter = 0;
}

int BandInterface::agregateCounter()
{
    return m_agregateCounter;
}


BandTitle * BandInterface::title()
{
    return m_titleItem;
}


QVariant BandInterface::itemChange ( GraphicsItemChange change, const QVariant & value )
{
    if (change == ItemSceneHasChanged && !value.isNull())
	if (scene())
	    this->createTitle( /*this->objectName()*/ this->metaObject()->className());
    if (change == ItemPositionHasChanged)
	m_titleItem->setPos(value.toPointF().x(), value.toPointF().y() - m_titleItem->boundingRect().height());
    return QGraphicsItem::itemChange(change, value);
}

}
