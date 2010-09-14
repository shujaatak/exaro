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

#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "rectangle.h"
#include "bandinterface.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(rectangle);
}

Rectangle::Rectangle(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject), m_xRadius(0), m_yRadius(0)
{
	initMyResource();
}

bool Rectangle::canContain(QObject * object)
{
	return (!dynamic_cast<Report::BandInterface*>(object) && dynamic_cast<Report::ItemInterface*>(object));
}

QRectF Rectangle::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

qreal Rectangle::xRadius()
{
	return m_xRadius;
}

void Rectangle::setXRadius(qreal radius)
{
	m_xRadius = radius;
	update();
}

qreal Rectangle::yRadius()
{
	return m_yRadius;
}

void Rectangle::setYRadius(qreal radius)
{
	m_yRadius = radius;
	update();
}


void Rectangle::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, rect);

	setupPainter(painter);
	adjustRect(rect);
	painter->drawRoundRect(rect, xRadius(), yRadius());
}

QIcon Rectangle::toolBoxIcon()
{
	return QIcon(":/rectangle.png");
};

QString Rectangle::toolBoxText()
{
	return tr("Rectangle");
}

QString Rectangle::toolBoxGroup()
{
	return tr("Shapes");
}

QObject * Rectangle::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Rectangle(parent, parentObject);
}

Q_EXPORT_PLUGIN2(rectangle, Rectangle)
