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
#include "dataset.h"
#include <qmath.h>

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
    , m_datasetFilterColumn (0)
    , darkRow(false)
    , m_isZebra(false)
    , m_columnAlignType(caHorizontal)
    , m_isFakePass (false)
//    , cashedData(0)
    , m_lastPage(-1)
{
	initMyResource();
	setResizeFlags(FixedPos | ResizeBottom);
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

    if ( (m_numColumns > 1) && (m_columnAlignType == caVertical) )
    {
	connect ( paintInterface, SIGNAL(processBandBefore(BandInterface*)), this, SLOT(checkCurrentBand(BandInterface*)), Qt::BlockingQueuedConnection);
	connect ( paintInterface, SIGNAL(closePageBefore()), this, SLOT(closePageBefore()), Qt::BlockingQueuedConnection);
    }

    return true;
}

bool Detail::prData()
{
    qDebug(" Detail::prData()");
    if ( !m_isFakePass )
	accumulateAgregateValues();

    if (m_numColumns > 1)
    {
	if ( (m_columnAlignType == caHorizontal) || ( (m_columnAlignType == caVertical)  &&  m_isFakePass ) )
	    m_currentColumn++;

	if ( m_currentColumn > m_numColumns || m_paintInterface->lastProcessedBand() != this)
	    m_currentColumn = 1;

	if (m_currentColumn > 1)
	{
	    if ( (m_columnAlignType == caHorizontal) || ( (m_columnAlignType == caVertical)  &&  m_isFakePass ) )
	    {
		QRectF freeSpace = m_paintInterface->pageFreeSpace();
		freeSpace.setTop( freeSpace.top() - geometry().height() - indentationTop() - indentationBottom() );
		m_paintInterface->setPageFreeSpace( freeSpace );
	    }
	}

	if (m_currentColumn == 1)
	    darkRow = !darkRow;

	    _offsetX = ( (qreal)geometry().width() / (qreal)m_numColumns ) * (qreal)(m_currentColumn - 1);
    }
    else
    {
	darkRow = !darkRow;
	m_currentColumn = 1;
    }
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

    m_currentColumn = 0;
    darkRow = false;
    offsetX = 0;
    _offsetX = 0;
    m_isFakePass = false;
    m_lastPage = -1;

    if ( (m_numColumns > 1) && (m_columnAlignType == caVertical) )
    {
	disconnect ( m_paintInterface, SIGNAL(processBandBefore(BandInterface*)), this, SLOT(checkCurrentBand(BandInterface*)));
	disconnect ( m_paintInterface, SIGNAL(closePageBefore()), this, SLOT(closePageBefore()));
    }
    cashedData.param.clear();
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

Detail::ColumnAlignType Detail::columnAlignType()
{
    return m_columnAlignType;
}

void Detail::setColumnAlignType (ColumnAlignType align)
{
    m_columnAlignType = align;
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
	if (dtst)
	{
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

void Detail::checkCurrentBand(Report::BandInterface* band)
{
    qDebug("check band = %s", qPrintable(band->objectName()));
    if ( (m_numColumns > 1) && (m_columnAlignType == caVertical) )
    {
	if (m_lastPage == m_paintInterface->currentPageNumber())
	{
	    if (m_paintInterface->lastProcessedBand() != this && band == this ) // first detail row
	    {
		qDebug("check band range start -----");
		cashedData.dsFirst = m_paintInterface->currentDatasetRow();
		m_isFakePass = true;
		qDebug("first row = %i", cashedData.dsFirst);
	    }
	    if (m_paintInterface->lastProcessedBand() == this && band != this)  // last detail row
	    {
		qDebug("check band range end -----");
		paintVerticalColumns();
	    }
	}
	else
	{
	    if (band == this)	// new page - so need new block
	    {
		qDebug("New Page detected - range begin -----");
		cashedData.dsFirst = m_paintInterface->currentDatasetRow();
		m_isFakePass = true;
		qDebug("first row = %i", cashedData.dsFirst);
	    }
	}

	if (band == this)
	    m_lastPage = m_paintInterface->currentPageNumber();
    }
}

void Detail::closePageBefore()
{
    qDebug("Detail::closePageBefore()===========================");
    if (m_isFakePass)
    {
	paintVerticalColumns();
	m_isFakePass = true;
	cashedData.dsFirst = m_paintInterface->currentDatasetRow() -1;
	this->prData();		// repeat lost (before new page) prData;
    }

//    this->prData();		// repeat lost (before new page) prData;

//    cashedData.dsFirst = m_paintInterface->currentDatasetRow();
//    m_isFakePass = true;
//    disconnect ( m_paintInterface, SIGNAL(processBandBefore(BandInterface*)), this, SLOT(checkCurrentBand(BandInterface*)));
}

/*
void Detail::closePageAfter()
{

}
*/

void Detail::paintVerticalColumns()
{
    qDebug("Detail::paintVerticalColumns()");
    Report::DataSet* dtst = this->reportObject()->findChild<Report::DataSet*> (m_dataset);
    Q_ASSERT(dtst);

    cashedData.dsLast = m_paintInterface->currentDatasetRow() - 1;

    m_isFakePass = false;
    m_currentColumn = 0;
    darkRow = false;

    int rows = qCeil ((double)cashedData.param.count() / (double)m_numColumns);
    qDebug("rows = %i, count = %i", rows, cashedData.param.count());
    for (int i = 0; i < cashedData.param.count() ; i++)
    {
	int col = i / rows;
	int trNum = (i - rows * col) * m_numColumns /*+ col*/;
	m_currentColumn = col + 1;
	qDebug("i=%i    currRow = %i   curCol = %i", i, trNum, col);
	dtst->seek( i + cashedData.dsFirst );
	m_paintInterface->setDetailNumber(cashedData.param.at(i).lineNum);
	if (this->prData())
	    this->prPaint(cashedData.param.at(i).painter, cashedData.param.at(trNum).translate, cashedData.param.at(i).clipRect);
    }
    dtst->seek(cashedData.dsLast + 1);

    cashedData.param.clear();
    m_currentColumn = 0;
    darkRow = false;
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
    qDebug("Detail::prPaint");
    if (m_isFakePass)
    {
	emit beforePrint(this);
	PrintParam pp;
	pp.painter = painter;
	pp.translate = translate;
	pp.clipRect = clipRect;
	pp.lineNum = m_paintInterface->currentDetailNumber();
	cashedData.param.append(pp);
	emit afterPrint(this);
	return true;
    }
    else
	return ItemInterface::prPaint(painter, translate, clipRect);
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
