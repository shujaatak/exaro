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

#include "legend.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(legend);
}

Legend::Legend(QGraphicsItem* parent, QObject* parentObject) : ChartInterface(parent, parentObject), m_legendColorRectWidth(5/UNIT)
{
	initMyResource();
}

QRectF Legend::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Legend::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit beforePrint(this);
	painter->save();
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);
	adjustRect(rect);

	QList<ChartInterface::_chartValue> val=values();

	if (!val.size())
	{
		if (option->type != QStyleOption::SO_GraphicsItem)
			emit afterPrint(this);
		painter->restore();
		return;
	}
	painter->fillRect(rect,brush());
	painter->drawRect(rect);
	painter->translate(rect.topLeft());
	painter->scale(rect.height()/((painter->fontMetrics().height()+2)*val.size()), rect.height()/((painter->fontMetrics().height()+2)*val.size()));
	int y=2;
	foreach (ChartInterface::_chartValue cv, val)
	{
		painter->fillRect(QRectF(2,y+1,m_legendColorRectWidth,painter->fontMetrics().height()),QBrush(cv.color));
		painter->drawText(QRectF(4+m_legendColorRectWidth,y+1,(rect.width()/(rect.height()/((painter->fontMetrics().height()+2)*val.size()))-(4+m_legendColorRectWidth)),painter->fontMetrics().height()+2),Qt::AlignVCenter|Qt::AlignLeft,cv.key);
		y+=painter->fontMetrics().height()+2;
	}
	painter->restore();
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit afterPrint(this);
}

QIcon Legend::toolBoxIcon()
{
	return QIcon(":/legend.png");
}

QString Legend::toolBoxText()
{
	return tr("Legend");
}

QString Legend::toolBoxGroup()
{
	return tr("Charts");
}

QObject * Legend::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Legend(parent, parentObject);
}

int Legend::legendColorRectWidth()
{
	return m_legendColorRectWidth;
}

void Legend::setLegendColorRectWidth(int legendColorRectWidth)
{
	if (m_legendColorRectWidth<1)
		legendColorRectWidth=1;
	m_legendColorRectWidth=legendColorRectWidth;
	update();
}

Q_EXPORT_PLUGIN2(legend, Legend)
