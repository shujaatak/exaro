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

#include "detailfooter.h"
#include "dataset.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(detailFooter);
}

DetailFooter::DetailFooter(QGraphicsItem* parent, QObject* parentObject): BandInterface(parent, parentObject)
{
	initMyResource();
	setResizeFlags(FixedPos | ResizeBottom);
}


DetailFooter::~DetailFooter()
{
}

bool DetailFooter::prInit(Report::PaintInterface * paintInterface)
{
    m_groupValue = "";
    m_agregateCounter = 0;
    needResetAgregateValues = true;
    return ItemInterface::prInit(paintInterface);
}

bool DetailFooter::prData()
{
    bool need_print = false;
    if (needResetAgregateValues)
	resetAgregateValues();

    accumulateAgregateValues();

    if (!m_condition.isEmpty() && !dataset().isEmpty())
    {
	m_groupValue = processString(m_condition);
	findDataset(dataset())->next();		    // query lookahead
	if (m_groupValue != processString(m_condition))
	    need_print = true;
	else
	    need_print = false;
	findDataset(dataset())->previous();		    //back
    }
    else
	need_print = true;

    if (need_print)
	needResetAgregateValues = true;
    else
	needResetAgregateValues = false;
    return need_print;
}

bool DetailFooter::canContain(QObject * object)
{
	return (!dynamic_cast<Report::BandInterface*>(object) && dynamic_cast<Report::ItemInterface*>(object));
}

QRectF DetailFooter::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}


//void DetailFooter::_paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
//{
//
//	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;
//
//	setupPainter(painter);
//	painter->fillRect(rect,painter->brush());
//
//	if (option->type == QStyleOption::SO_GraphicsItem)
//	{
//		drawSelection(painter, rect);
//		drawTitle(tr("Detail footer"), TitleLeft, Qt::AlignCenter);
//	}
//
////	adjustRect(rect);
//
//	if (frame()&DrawLeft)
//		painter->drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());
//
//	if (frame()&DrawRight)
//		painter->drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
//
//	if (frame()&DrawTop)
//		painter->drawLine(rect.left(), rect.top(), rect.right(), rect.top());
//
//	if (frame()&DrawBottom)
//		painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
//
//}


QIcon DetailFooter::toolBoxIcon()
{
	return QIcon(":/detailFooter.png");
}

QString DetailFooter::toolBoxText()
{
	return tr("Detail footer");
}

QString DetailFooter::toolBoxGroup()
{
	return tr("Bands");
}

QObject * DetailFooter::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new DetailFooter(parent, parentObject);
}

QString DetailFooter::condition()
{
    return m_condition;
}

void DetailFooter::setCondition(const QString & condition)
{
    m_condition = condition;
}

Q_EXPORT_PLUGIN2(detailFooter, DetailFooter)

