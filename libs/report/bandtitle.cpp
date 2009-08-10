/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 *   Copyright (C) 2009 by Mikhalov Alexaner                               *
 *   alexmi3@rambler.ru                                                    *
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

#include "bandtitle.h"

namespace Report
{

BandTitle::BandTitle(BandInterface * parentBand, const QSizeF & size, const QString & text, QGraphicsItem * parent):
		QGraphicsItem(parent),m_parentBand(parentBand),m_size(size), m_text(text)
{
}

void BandTitle::setSize(QSizeF size)
{
	prepareGeometryChange ();
	if (m_size==size)
		return;
	m_size=size;
	update();
}

QRectF BandTitle::boundingRect() const
{
	return QRectF(0,0,m_size.width(), m_size.height());
}

void BandTitle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QRectF rect=boundingRect();
//	qreal penwidth = painter->pen().widthF();
//	rect=rect.adjusted(penwidth,penwidth,-penwidth,-penwidth);
	painter->save();
	QFont f=painter->font();
	f.setPointSizeF(20);
	f.setBold(true);
	painter->setFont(f);
	painter->setPen(QColor(224,224,224));
	QBrush a;
//	a.setColor(QColor(194,255,240,50));
	a.setColor(QColor(224,224,224));
	a.setStyle(Qt::SolidPattern);
//	painter->fillRect(rect, QColor(0,0,40));
	QPainterPath p;
		p.moveTo(0, 40);
		p.lineTo(40, 0);
		p.lineTo(rect.width() - 40, 0);
		p.lineTo(rect.width(), rect.height());
		painter->drawPath(p);
		painter->fillPath(p,a);
//		painter->rotate(270);
		painter->setPen(QColor(50,50,50));
//		painter->drawText(rect, Qt::AlignCenter, m_text);
		painter->drawText(40,0, rect.width() - 40, rect.height(), Qt::AlignLeft, m_text);
	painter->restore();
}

void BandTitle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	m_parentBand->selectItem();
}

void BandTitle::setCaption(QString str)
{
    m_text = str;
    update();
}

QString BandTitle::caption()
{
    return m_text;
}

}
