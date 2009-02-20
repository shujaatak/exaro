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
#include "ellipse.h"


inline void initMyResource()
{
	Q_INIT_RESOURCE(ellipse);
}

Ellipse::Ellipse(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject)
{
	initMyResource();

}

bool Ellipse::canContain(QObject * object)
{
	return (!dynamic_cast<Report::BandInterface*>(object) && dynamic_cast<Report::ItemInterface*>(object));
}

QRectF Ellipse::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Ellipse::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);
	adjustRect(rect);
	painter->drawEllipse(rect);
}

QIcon Ellipse::toolBoxIcon()
{
	return QIcon(":/ellipse.png");
}

QString Ellipse::toolBoxText()
{
	return tr("Ellipse");
}

QString Ellipse::toolBoxGroup()
{
	return tr("Shapes");
}

QObject * Ellipse::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Ellipse(parent, parentObject);
}

Q_EXPORT_PLUGIN2(ellipse, Ellipse)
