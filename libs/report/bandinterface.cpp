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
		BandInterface * m_parentBand;
		QSizeF m_size;
		QString m_text;
		int m_textFlags;
		BandInterface::TitlePosition m_position;
};

TitleItem::TitleItem(BandInterface * parentBand, const QSizeF & size, const QString & text, int textFlags, BandInterface::TitlePosition position, QGraphicsItem * parent):
		QGraphicsItem(parent),m_parentBand(parentBand),m_size(size), m_text(text), m_textFlags(textFlags), m_position(position)
{
}

void TitleItem::setSize(QSizeF size)
{
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
		: ItemInterface(parent, parentObject), 
		m_reprintOnNewPage(false), m_deleting(false), 
		m_resetDetailNumber(false),m_forceNewPage(false),
		m_titleItem(0)
{
	setResizeFlags(FixedPos | ResizeTop | ResizeBottom);

	if (!parentObject)
		return;

	if (dynamic_cast<ItemInterface*>(parent))
		connect(dynamic_cast<ItemInterface*>(parent), SIGNAL(geometryChanged(QRectF)), this, SLOT(updateGeometry(QRectF)));
	else
		connect(parentObject, SIGNAL(geometryChanged(QRectF)), this, SLOT(updateGeometry(QRectF)));

	connect(this, SIGNAL(bandDelete(int, int)), parentObject, SLOT(bandDestroyed(int, int)));

	m_order = -1;
	m_indentation = 0;
	m_frame = 0;
	m_query = "";
	m_bandType=Overlay;

	setHeight(50/UNIT);
}

void BandInterface::selectBand()
{
	emit(itemSelected(this, QPointF(0,0)));
}

bool BandInterface::deleting()
{
	return m_deleting;
}

void BandInterface::removeItem()
{
	m_deleting = true;
	emit(bandDelete(m_bandType, m_order));
        ItemInterface::removeItem();
}

BandInterface::~BandInterface()
{
	delete m_titleItem;
}

BandInterface::Frames BandInterface::frame()
{
	return m_frame;
}

void BandInterface::setFrame(Frames frame)
{
	m_frame = frame;
	update();
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

int BandInterface::order()
{
	return m_order;
}

void BandInterface::setHeight(qreal height)
{
	ItemInterface::setHeight(height);

	if (!dynamic_cast<Report::ItemInterface*>(parentItem()) && !scene())
		return;

	QList<QGraphicsItem *> lc;

	if (dynamic_cast<Report::ItemInterface*>(parentItem()))
		lc = dynamic_cast<Report::ItemInterface*>(parentItem())->childItems();
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
			m_titleItem->setVisible(false);
			m_titleItem->setSize(QSizeF(40,geometry().height()));
			m_titleItem->setPos(pos);
			m_titleItem->setVisible(true);
		}
	}
}

void BandInterface::setGeometry(QRectF rect)
{
	if (bandType() == Overlay)
	{
		ItemInterface::setGeometry(rect/*&parentGeometry()*/);
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


BandInterface::BandType BandInterface::bandType()
{
	return m_bandType;
}

void BandInterface::setBandType(BandType bandType)
{
	m_bandType = bandType;
}

QString BandInterface::query()
{
	return m_query;
}

void BandInterface::setQuery(const QString & query)
{
	m_query = query;
}

bool BandInterface::reprintOnNewPage()
{
	return m_reprintOnNewPage;
}

void BandInterface::setReprintOnNewPage(bool reprintOnNewPage)
{
	m_reprintOnNewPage = reprintOnNewPage;
}

bool BandInterface::resetDetailNumber()
{
	return m_resetDetailNumber;
}
void BandInterface::setResetDetailNumber(bool resetDetailNumber)
{
	m_resetDetailNumber=resetDetailNumber;
}

QString BandInterface::groupField()
{
	return m_groupField;
}

void BandInterface::setGroupField(const QString & groupField)
{
	m_groupField = groupField;
}

QVariant BandInterface::groupFieldValue()
{
	return m_groupFieldValue;
}

void BandInterface::setGroupFieldValue(const QVariant & groupFieldValue)
{
	m_groupFieldValue = groupFieldValue;
}

bool BandInterface::forceNewPage()
{
	return m_forceNewPage;
}
void BandInterface::setForceNewPage(bool forceNewPage)
{
	m_forceNewPage=forceNewPage;
}

void BandInterface::bandDestroyed(int type, int order)
{
	foreach(QObject *item, QObject::children())
		if (dynamic_cast<BandInterface*>(item))
		{
			if (dynamic_cast<BandInterface*>(item)->bandType() == type && dynamic_cast<BandInterface*>(item)->order() > order)
				dynamic_cast<BandInterface*>(item)->setOrder(dynamic_cast<BandInterface*>(item)->order() - 1);
			if (dynamic_cast<BandInterface*>(item)->bandType() > type)
					dynamic_cast<BandInterface*>(item)->setOrder(dynamic_cast<BandInterface*>(item)->order());
		}
	update();
}

}
