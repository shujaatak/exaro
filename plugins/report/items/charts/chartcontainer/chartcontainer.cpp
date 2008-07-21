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
#include "chartcontainer.h"

using namespace Report;

inline void initMyResource()
{
	Q_INIT_RESOURCE(chartcontainer);
}

ChartContainer::ChartContainer(QGraphicsItem* parent, QObject* parentObject) : ChartDataCollector(parent, parentObject) 
{
	initMyResource();
}

bool ChartContainer::canContain(QObject * object)
{
	return dynamic_cast<Report::ChartInterface*>(object);
}

QRectF ChartContainer::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void ChartContainer::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit beforePrint(this);

	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
	{
		drawSelection(painter, boundingRect());
		setupPainter(painter);
		adjustRect(rect);
		painter->drawText(rect,Qt::AlignBottom|Qt::AlignRight,tr("Chart container"));
	}
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit afterPrint(this);
}

QIcon ChartContainer::toolBoxIcon()
{
	return QIcon(":/chartcontainer.png");
}

QString ChartContainer::toolBoxText()
{
	return tr("Chart container");
}

QString ChartContainer::toolBoxGroup()
{
	return tr("Charts");
}

QObject * ChartContainer::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new ChartContainer(parent, parentObject);
}

Q_EXPORT_PLUGIN2(chartContainer, ChartContainer)
