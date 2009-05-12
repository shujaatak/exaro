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

#include "detailheader.h"
#include <paintinterface.h>

inline void initMyResource()
{
	Q_INIT_RESOURCE(detailHeader);
}

DetailHeader::DetailHeader(QGraphicsItem* parent, QObject* parentObject)
    : BandInterface(parent, parentObject),
	m_reprintOnNewPage(false), m_resetDetailNumber(false), m_forceNewPage(false),
	m_first(true), m_lastRowPrinted(-1)
{
	initMyResource();
	setResizeFlags(FixedPos | ResizeBottom);
}


DetailHeader::~DetailHeader()
{
}

bool DetailHeader::prInit(Report::PaintInterface * paintInterface)
{
    m_lastRowPrinted = -1;
    m_groupValue = "";
//    return ItemInterface::init(paintInterface);
}
/*
bool DetailHeader::prePaint(QPainter * painter, Report::PaintInterface::PrintMode pMode)
{
    ItemInterface::prePaint(painter);
    bool ok = false;
    switch (pMode)
    {
	case Report::PaintInterface::pmNewPage:
	    if (m_reprintOnNewPage && !m_forceNewPage)
	    {
		m_lastRowPrinted = m_paintInterface->currentDatasetRow();
		return true;
	    }
	    break;
	case Report::PaintInterface::pmNormal:
//	qDebug("currentQueryRow()=%i         m_lastRowPrinted =%i", m_paintInterface->currentQueryRow(), m_lastRowPrinted);
//	qDebug("m_groupValue =%s   processString(m_condition)=%s", qPrintable(m_groupValue), qPrintable(processString(m_condition)));
	    if (m_paintInterface->currentDatasetRow() != m_lastRowPrinted) //prevent doubling in new page
		if (m_condition.isEmpty() || (!m_condition.isEmpty() && m_groupValue != processString(m_condition)))
			ok = true;
	    if (ok)
	    {
		m_lastRowPrinted = m_paintInterface->currentDatasetRow();

		if (m_resetDetailNumber)
		    m_paintInterface->setDetailNumber(1);
		if (m_forceNewPage && m_paintInterface->currentDatasetRow() !=1)
		    m_paintInterface->newPage();
		if (!m_condition.isEmpty())
		    m_groupValue = processString(m_condition);
		return true;
	    }
	    break;
	default:
	    return false;
    }

    return false;
}
*/
QString DetailHeader::condition()
{
    return m_condition;
}

void DetailHeader::setCondition(const QString & condition)
{
    m_condition = condition;
}

bool DetailHeader::reprintOnNewPage()
{
	return m_reprintOnNewPage;
}

void DetailHeader::setReprintOnNewPage(bool reprintOnNewPage)
{
	m_reprintOnNewPage = reprintOnNewPage;
}

bool DetailHeader::forceNewPage()
{
	return m_forceNewPage;
}
void DetailHeader::setForceNewPage(bool forceNewPage)
{
	m_forceNewPage=forceNewPage;
}

bool DetailHeader::resetDetailNumber()
{
	return m_resetDetailNumber;
}
void DetailHeader::setResetDetailNumber(bool resetDetailNumber)
{
	m_resetDetailNumber=resetDetailNumber;
}

bool DetailHeader::canContain(QObject * object)
{
	return (!dynamic_cast<Report::BandInterface*>(object) && dynamic_cast<Report::ItemInterface*>(object));
}


QRectF DetailHeader::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void DetailHeader::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit beforePrint(this);

	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	setupPainter(painter);
	painter->fillRect(rect,painter->brush());

	if (option->type == QStyleOption::SO_GraphicsItem)
	{
		drawSelection(painter, rect);
		drawTitle(tr("Detail header"), TitleLeft, Qt::AlignCenter);
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


	if (option->type != QStyleOption::SO_GraphicsItem)
		emit afterPrint(this);
}

QIcon DetailHeader::toolBoxIcon()
{
	return QIcon(":/detailHeader.png");
}

QString DetailHeader::toolBoxText()
{
	return tr("Detail header");
}

QString DetailHeader::toolBoxGroup()
{
	return tr("Bands");
}

QObject * DetailHeader::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new DetailHeader(parent, parentObject);
}

Q_EXPORT_PLUGIN2(detailHeader, DetailHeader)

