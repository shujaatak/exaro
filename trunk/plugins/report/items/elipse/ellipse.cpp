/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bogdan@licentia.eu                                                    *
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
#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "bandinterface.h"
#include "ellipse.h"


inline void initMyResource()
{
	Q_INIT_RESOURCE(elipse);
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
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit beforePrint(this);

	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);

	adjustRect(rect);

	painter->drawEllipse(rect);

	if (option->type != QStyleOption::SO_GraphicsItem)
		emit afterPrint(this);

}

QIcon Ellipse::toolBoxIcon()
{
	return QIcon(":/elipse.png");
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
