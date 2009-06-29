/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *       bog_dan_ro@yahoo.com                                              *
 *   Copyright (C) 2009 by Alexander Mikhalov                              *
 *       alexmi3@rambler.ru                                                *
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
#include "reportinterface.h"

#include "detail.h"

#define  MAX_COLUMNS 5

inline void initMyResource()
{
	Q_INIT_RESOURCE(detail);
}

Detail::Detail(QGraphicsItem* parent, QObject* parentObject)
    : BandInterface(parent, parentObject)
    , m_numColumns(1)
    , m_currentColumn (0)
{
	initMyResource();
	setResizeFlags(FixedPos | ResizeBottom);
	m_isZebra = true;
	darkRow = false;
	this->m_datasetFilterColumn = 0;
}

Detail::~Detail()
{
}

bool Detail::prInit(Report::PaintInterface * paintInterface)
{
    Report::ItemInterface::prInit(paintInterface);
    if (!m_joinTo.isEmpty())
    {
	Report::BandInterface * band = this->reportObject()->findChild<Report::BandInterface *>(m_joinTo);
	if (!band)
	    return true;
	connect ( band, SIGNAL (afterPrint(QObject *)) , this, SLOT (joinToSlot(QObject *)) );
    }
    return true;
}

bool Detail::prData()
{
    accumulateAgregateValues();

    if (m_numColumns > 1)
    {
	m_currentColumn++;
	if ( m_currentColumn > m_numColumns || m_paintInterface->lastProcessedBand() != this)
	    m_currentColumn = 1;

	if (m_currentColumn > 1)
	{
	    QRectF freeSpace = m_paintInterface->pageFreeSpace();
	    freeSpace.setTop( freeSpace.top() - geometry().height() - indentation() );
	    m_paintInterface->setPageFreeSpace( freeSpace );
	}

	if (m_currentColumn == 1)
	    darkRow = !darkRow;
    }
    else
	darkRow = !darkRow;

    return true;
}


bool Detail::prReset()
{
    resetAgregateValues();
    if (!m_joinTo.isEmpty())
    {
	Report::BandInterface * band = this->reportObject()->findChild<Report::BandInterface *>(m_joinTo);
	if (!band)
	    return true;
	disconnect ( band, SIGNAL (afterPrint(QObject *)) , this, SLOT (joinToSlot(QObject *)) );
    }
//    if (!this->m_dataset.isEmpty())
//	Report::DataSet * dtst = this->reportObject()->findChild<Report::BandInterface *>(m_joinTo);
    m_currentColumn = 0;
    darkRow = false;

    return true;
}

bool Detail::isZebra()
{
    return m_isZebra;
}

void Detail::setZebra(bool b)
{
    m_isZebra = b;
}

QString Detail::joinTo()
{
    return m_joinTo;
}

void Detail::setJoinTo(QString bandName)
{
    m_joinTo = bandName;
}

int Detail::numColumns()
{
    return m_numColumns;
}

void Detail::setNumColumns(int col)
{
    m_numColumns = col;
    if (m_numColumns < 1)
	m_numColumns = 1;
    if (m_numColumns > MAX_COLUMNS)
	m_numColumns = MAX_COLUMNS;
    update();
}

QString Detail::datasetFilter()
{
    return m_datasetFilter;
}

void Detail::setDatasetFilter(QString filter)
{
    m_datasetFilter = filter;
}

int Detail::datasetFilterColumn()			// count 1...n, 0 - not defined, -1 for all columns
{
    return this->m_datasetFilterColumn;
}

void Detail::setDatasetFilterColumn(int column)
{
    this->m_datasetFilterColumn = column;
}

void Detail::joinToSlot(QObject * item)
{
    darkRow = false;
    Report::DataSet * dtst = this->m_dataset.isEmpty() ? 0 : this->reportObject()->findChild<Report::DataSet *>(this->m_dataset);
    if (!m_datasetFilter.isEmpty() && !(m_datasetFilterColumn == 0))
    {
	qDebug("test 1");
	if (dtst)
	{
		qDebug("test 2");
	    QString filter = m_datasetFilter;
	    QString regExp ("\\$([\\w\\d]+)");
	    QRegExp rxlen(regExp);
	    int pos = 0;
	    while ((pos = rxlen.indexIn(filter, pos)) != -1)
	    {
		QString _fieldName = rxlen.cap(0);
		QString fieldName = rxlen.cap(1);
		filter.replace(_fieldName,  this->m_paintInterface->currentDataset()->value(fieldName).toString());
		pos += rxlen.matchedLength();
	    }
	    qDebug("filter = %s", qPrintable(filter));
	    if (m_datasetFilterColumn == -1)
		 dtst->setFilter(m_datasetFilterColumn, filter);
	    else
		dtst->setFilter(m_datasetFilterColumn - 1, filter);
	}
    }
    if (dtst)
	m_paintInterface->processDataset(dtst);
    else
	m_paintInterface->processBand(this);
}

bool Detail::canContain(QObject * object)
{
	return (!dynamic_cast<Report::BandInterface*>(object) && dynamic_cast<Report::ItemInterface*>(object));
}

QRectF Detail::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Detail::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	setupPainter(painter);

	painter->fillRect(rect,painter->brush());

	if ( option->type != QStyleOption::SO_GraphicsItem &&  darkRow && m_currentColumn  <= 1)
	    painter->fillRect(rect,QBrush(QColor(0,0,0,20)));

	if (option->type == QStyleOption::SO_GraphicsItem)
	{
		drawSelection(painter, rect);
		drawTitle(tr("Detail"), TitleLeft, Qt::AlignCenter);
		if (m_numColumns > 1 )
		{
		    QPen p(Qt::DashLine);
		    painter->setPen(p);
		    qreal deltaX = (qreal)rect.width() / (qreal)m_numColumns;
		    for (int i=1 ;i<=m_numColumns; i++)
			painter->drawLine(rect.left() + deltaX*i, rect.top(), rect.left() + deltaX*i, rect.bottom() );
		}
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

	if (option->type == QStyleOption::SO_GraphicsItem &&  m_currentColumn > 1)
	    painter->translate((qreal)rect.width() / (qreal)m_numColumns , 0);
}


bool Detail::prPaint(QPainter * painter, QPointF translate, const QRectF & clipRect)
{
    emit beforePrint(this);

    QStyleOptionGraphicsItem option;
    option.type = 31;
    option.exposedRect = dynamic_cast<BandInterface *>(this) ? QRectF(0, 0, dynamic_cast<BandInterface *>(this)->geometry().width(), dynamic_cast<BandInterface *>(this)->geometry().height()) : geometry();
    painter->save();
    option.exposedRect.translate(translate);
    painter->setClipRect(clipRect);

    paint(painter, &option);

    painter->restore();
    translate += option.exposedRect.topLeft() + ((QPointF((qreal)geometry().width() / (qreal)m_numColumns , 0)) * (m_currentColumn - 1));
    foreach(ItemInterface * childItem, findChildren<ItemInterface *>())
	if (childItem->prData())
	    childItem->prPaint(painter, translate, option.exposedRect);

    emit afterPrint(this);
}


QIcon Detail::toolBoxIcon()
{
	return QIcon(":/detail.png");
}

QString Detail::toolBoxText()
{
	return tr("Detail");
}

QString Detail::toolBoxGroup()
{
	return tr("Bands");
}

QObject * Detail::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Detail(parent, parentObject);
}

Q_EXPORT_PLUGIN2(detail, Detail)
