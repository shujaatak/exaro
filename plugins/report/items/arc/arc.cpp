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

#include "bandinterface.h"
#include "arc.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(arc);
}

Arc::Arc(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject), m_startAngle(30), m_spanAngle(120)
{
	initMyResource();

}

bool Arc::canContain(QObject * object)
{
	return (!dynamic_cast<Report::BandInterface*>(object) && dynamic_cast<Report::ItemInterface*>(object));
}

int Arc::startAngle()
{
	return m_startAngle;
}

void Arc::setStartAngle(int startAngle)
{
	m_startAngle = startAngle;
	update();
}

int Arc::spanAngle()
{
	return m_spanAngle;
}

void Arc::setSpanAngle(int spanAngle)
{
	m_spanAngle = spanAngle;
	update();
}

QRectF Arc::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Arc::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);
	adjustRect(rect);
	painter->drawArc(rect, m_startAngle*16, m_spanAngle*16);

}

QIcon Arc::toolBoxIcon()
{
	return QIcon(":/arc.png");
}

QString Arc::toolBoxText()
{
	return tr("Arc");
}

QString Arc::toolBoxGroup()
{
	return tr("Shapes");
}

QObject * Arc::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Arc(parent, parentObject);
}

Q_EXPORT_PLUGIN2(arc, Arc)
