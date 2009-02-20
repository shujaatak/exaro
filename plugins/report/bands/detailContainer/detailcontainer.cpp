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
	if (option->type == QStyleOption::SO_GraphicsItem)
	{
		QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;
		setupPainter(painter);
		drawSelection(painter, rect);
		drawTitle(tr("DetailContainer"), TitleRight, Qt::AlignCenter);
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

