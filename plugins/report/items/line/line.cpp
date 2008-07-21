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
#include <QPointF>
#include <QStyleOptionGraphicsItem>

#include "line.h"


inline void initMyResource()
{
	Q_INIT_RESOURCE(line);
}

Line::Line(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject), m_lineStyle(BackwardDiagonal)
{
	initMyResource();

}


QRectF Line::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

Line::LineStyle Line::lineStyle()
{
	return m_lineStyle;
}

void Line::setLinestyle(LineStyle lineStyle)
{
	m_lineStyle = lineStyle;
	update();
}

void Line::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit beforePrint(this);

	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);

	adjustRect(rect);

	switch (lineStyle())
	{
		case Vertical:
			painter->drawLine(rect.x() + rect.width() / 2, rect.y(), rect.x() +  rect.width() / 2, rect.y()+rect.height());
			break;
		case Horizontal:
			painter->drawLine(rect.x(), rect.y() + rect.height() / 2, rect.x()+rect.width(), rect.y()+ rect.height() / 2);
			break;
		case BackwardDiagonal:
			painter->drawLine(rect.right(), rect.y(), rect.x(), rect.bottom());
			break;
		case ForwardDiagonal:
			painter->drawLine(rect.x(), rect.y(), rect.right(), rect.bottom());
			break;
	}

	if (option->type != QStyleOption::SO_GraphicsItem)
		emit afterPrint(this);
}

QIcon Line::toolBoxIcon()
{
	return QIcon(":/line.png");
}

QString Line::toolBoxText()
{
	return tr("Line");
}

QString Line::toolBoxGroup()
{
	return tr("Shapes");
}

QObject * Line::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Line(parent, parentObject);
}

Q_EXPORT_PLUGIN2(line, Line)
