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

class TitleItem : public QGraphicsItem
{
	public:
		TitleItem( BandInterface * parentBand, const QSizeF & size, const QString & text, int textFlags, BandInterface::TitlePosition position,  QGraphicsItem * parent=0 );
		void setSize(QSizeF size);
		inline const QSizeF & size(){return m_size;};
		QRectF boundingRect() const;
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *event);

	private:
		QSizeF m_size;
		QString m_text;
		int m_textFlags;
		BandInterface::TitlePosition m_position;
		BandInterface * m_parentBand;
};

TitleItem::TitleItem(BandInterface * parentBand, const QSizeF & size, const QString & text, int textFlags, BandInterface::TitlePosition position, QGraphicsItem * parent):
		QGraphicsItem(parent),m_parentBand(parentBand),m_size(size), m_text(text), m_textFlags(textFlags), m_position(position)
{
}

void TitleItem::setSize(QSizeF size)
{
	prepareGeometryChange ();
	if (m_size==size)
		return;
	m_size=size;
	update();
}

QRectF TitleItem::boundingRect() const
{
	return QRectF(0,0,m_size.width(), m_size.height());
}

void TitleItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QRectF rect=boundingRect();
	qreal penwidth = painter->pen().widthF();
	rect=rect.adjusted(penwidth,penwidth,-penwidth,-penwidth);
	painter->save();
	QFont f=painter->font();
	f.setPointSizeF(16);
	painter->setFont(f);
	painter->setPen(QColor(224,224,224));
	QBrush a;
	a.setColor(QColor(194,255,240,50));
	a.setStyle(Qt::SolidPattern);
	QPainterPath p;
	if (m_position==BandInterface::TitleLeft)
	{
		p.moveTo(40,0);
		p.lineTo(0,40);
		p.lineTo(0,rect.height()-40);
		p.lineTo(40,rect.height());
		painter->drawPath(p);
		painter->fillPath(p,a);
		painter->rotate(270);
		painter->setPen(QColor(50,50,50));
		painter->drawText(-rect.height(), 0, rect.height(), 40, m_textFlags, m_text);
	}
	else
	{
		p.moveTo(0,0);
		p.lineTo(40,40);
		p.lineTo(40,rect.height()-40);
		p.lineTo(0,rect.height());
		painter->drawPath(p);
		painter->fillPath(p,a);
		painter->rotate(90);
		painter->drawText(0, -40, rect.height(), 40, m_textFlags, m_text);
	}
	painter->restore();
}

void TitleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	m_parentBand->selectBand();
}


BandInterface::BandInterface(QGraphicsItem* parent, QObject * parentObject)
		: ItemInterfaceExt(parent, parentObject),
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
	m_indentation = 0;
	m_query = "";

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

void BandInterface::setIndentation(int indentation)
{
	if (indentation < 0)
		indentation = 0;

	m_indentation = indentation;
	setHeight(height());
}

int BandInterface::indentation()
{
	return m_indentation;
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

QString BandInterface::query()
{
	return m_query;
}

void BandInterface::setQuery(const QString & query)
{
	m_query = query;
}

void BandInterface::addAgregateValue(QString value)
{
    if (!m_agregateValues.contains(value))
    {
	ValueStruct vStruct;
	if (stringIsField(value))
	    stringToField (value, &vStruct.query, &vStruct.field);
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
	if (vStruct->query.isEmpty())
	    vStruct->list.append( scriptEngine()->globalObject().property(value).toNumber() );
	else
	    vStruct->list.append( queryField(vStruct->query, vStruct->field).toDouble() );

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

}
