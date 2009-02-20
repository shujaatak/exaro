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

#include "overlay.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(overlay);
}

Overlay::Overlay(QGraphicsItem* parent, QObject* parentObject): BandInterface(parent, parentObject)
{
	initMyResource();
	setBandType(Report::BandInterface::Overlay);
	setResizeFlags(ResizeLeft | ResizeTop | ResizeRight | ResizeBottom);
	setHeight(50/UNIT);
	setWidth(50/UNIT);
}


Overlay::~Overlay()
{
}

bool Overlay::canContain(QObject * object)
{
	return (!dynamic_cast<Report::BandInterface*>(object) && dynamic_cast<Report::ItemInterface*>(object));
}

QRectF Overlay::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Overlay::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	setupPainter(painter);

	painter->fillRect(rect, painter->brush());

	if (option->type == QStyleOption::SO_GraphicsItem)
	{
		drawSelection(painter, rect);
		painter->setPen(QColor(225,224,224));
		painter->drawText(rect, tr("Overlay"));
	}

	adjustRect(rect);

	if (frame()&DrawLeft)
		painter->drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());

	if (frame()&DrawRight)
		painter->drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());

	if (frame()&DrawTop)
		painter->drawLine(rect.left(), rect.top(), rect.right(), rect.top());

	if (frame()&DrawBottom)
		painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
}

QIcon Overlay::toolBoxIcon()
{
	return QIcon(":/overlay.png");
}

QString Overlay::toolBoxText()
{
	return tr("Overlay");
}

QString Overlay::toolBoxGroup()
{
	return tr("Bands");
}

QObject * Overlay::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Overlay(parent, parentObject);
}

Q_EXPORT_PLUGIN2(overlay, Overlay)

