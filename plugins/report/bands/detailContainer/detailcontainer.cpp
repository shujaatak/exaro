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

#include "detailcontainer.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(detailContainer);
}

DetailContainer::DetailContainer(QGraphicsItem* parent, QObject* parentObject): BandInterface(parent, parentObject)
{
	initMyResource();
	setBandType(Report::BandInterface::DetailContainer);
	setResizeFlags(FixedPos | ResizeBottom);
	setHeight(150/UNIT);
}



bool DetailContainer::canContain(QObject * object)
{
	return (dynamic_cast<Report::BandInterface*>(object) && (dynamic_cast<Report::BandInterface*>(object)->bandType() == Report::BandInterface::DetailHeader || dynamic_cast<Report::BandInterface*>(object)->bandType() == Report::BandInterface::Detail || dynamic_cast<Report::BandInterface*>(object)->bandType() == Report::BandInterface::DetailFooter));
}

QRectF DetailContainer::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void DetailContainer::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
	{
		setupPainter(painter);
		drawSelection(painter, rect);
		painter->drawText(boundingRect(), tr("DetailContainer"));
	}

}

QIcon DetailContainer::toolBoxIcon()
{
	return QIcon(":/detailContainer.png");
}

QString DetailContainer::toolBoxText()
{
	return tr("DetailContainer");
}

QString DetailContainer::toolBoxGroup()
{
	return tr("Bands");
}

QObject * DetailContainer::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new DetailContainer(parent, parentObject);
}

Q_EXPORT_PLUGIN2(detailContainer, DetailContainer)

