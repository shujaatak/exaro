/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2009 by Alexander Mikhalov  (aka alFoX)                 *
 *              alexmi3@rambler.ru                                         *
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


#include "paintinterface.h"
#include <QtSql>
#include <paintdevice.h>
#include "reportinterface.h"
#include "pageinterface.h"
#include "dataset.h"


using namespace Report;

PaintInterface::PaintInterface(ReportInterface * report, QObject * parent)
	:QThread(parent)
{
    m_report = report;
    m_currentDataset = 0;
    m_lastProcessedBand = 0;
}

PaintInterface::~PaintInterface()
{
    delete m_printer;
}

void PaintInterface::run()
{
    emit showProcess(tr("Init painter..."));
    m_report->m_doc.appendChild(m_report->m_doc.createComment("Author '" + m_report->author() + "'"));

    #warning 'FIXME: review creating file in same thread'
    m_report->pdf_file = new QTemporaryFile(QDir::tempPath()+"/XXXXXXXXXXXXX.bdrtpf", m_report);
    if (!m_report->pdf_file->open())
	throw QString(tr("Can't create temporary files"));

    m_printer = new PaintDevice(m_report->pdf_file);
    m_report->m_exportNode = m_report->m_doc.createElement("export");
    m_painter.begin(m_printer);
    m_painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);


//    bool first=true;
    foreach (PageInterface* page, m_report->findChildren<PageInterface *>())
    {
//	if (!first)
//	    m_printer->newPage();
	m_printer->setPaperSize(page->paperRect().size());
	m_printer->setPaperOrientation((QPrinter::Orientation)page->orientation());
	m_currentPage = page;
	processTemplatePage();
    }

    m_painter.end();
    delete m_printer;
    m_printer = 0;
}

void PaintInterface::processTemplatePage()
{
    listTop.clear();
    listBottom.clear();
    listFree.clear();

    freeSpace = QRect();

    LayoutManager::splitOnLayoutTypesSorted(m_currentPage, &listTop, &listBottom, &listFree);

    initBands();

    m_currentPageNumber = 1;
    m_report->m_scriptEngine->globalObject().setProperty("_page_", QScriptValue(m_report->m_scriptEngine, 1), QScriptValue::ReadOnly);

    prepareCurrentPage();

    bandDone.clear();

    for (int i = listBottom.count()-1; i>=0 ;i--)
    {
	BandInterface * band = listBottom.at(i);
	if (band->prData())
	    processBand(band);
    }

    foreach(BandInterface * band, listTop)
    {
	qDebug ("turn process = %s", qPrintable(band->objectName()));
	if (!bandDone.contains(band))
	{
	    if (band->dataset().isEmpty())
	    {
		if ( band->prData() )
		    processBand(band);
	    }
	    else    /// process dataset group
	    {
		DataSet * dtst = m_report->findChild<DataSet *>(band->dataset());
		if (!dtst)
		    finish(tr("Dataset named \'%1\' not found for band \'%2\'").arg(dtst->objectName()).arg(band->objectName()) );

		processDataset(dtst);
	    }
	}
    }

    postprocessCurrentPage();

    /// reset temporary data in items
    foreach (ItemInterface * item, m_report->findChildren <ItemInterface *>())
	item->prReset();
}

void PaintInterface::finish(QString error)
{
    showError(error);
    deleteLater () ;
}

void PaintInterface::initBands()
{
    foreach(BandInterface * band, listTop)
	if (!band->prInit(this))
	    showError(QString("Error in item \'%1\' \n%2").arg(band->objectName()).arg(band->lastError()));
    	else
	    foreach (Report::ItemInterface* item , band->findChildren<ItemInterface *>())
		item->prInit(this);

    foreach(BandInterface * band, listBottom)
	if (!band->prInit(this))
	    showError(QString("Error in item \'%1\' \n%2").arg(band->objectName()).arg(band->lastError()));
    	else
	    foreach (Report::ItemInterface* item , band->findChildren<ItemInterface *>())
		item->prInit(this);

    foreach(BandInterface * band, listFree)
	if (!band->prInit(this))
	    showError(QString("Error in item \'%1\' \n%2").arg(band->objectName()).arg(band->lastError()));
    	else
	    foreach (Report::ItemInterface* item , band->findChildren<ItemInterface *>())
		item->prInit(this);
}


void PaintInterface::showError(QString err)
{
    qDebug("Error:\n %s", qPrintable(err));
}

void PaintInterface::processBand(BandInterface * band/*, PrintMode pMode*/)
{
    if (!band /*|| m_reportCanceled*/)
	return;
    qDebug("PaintInterface::processBand = %s", qPrintable(band->objectName()));
    if (!band->isEnabled())
	return;

    emit processBandBefore(band);

    m_painter.save();
    m_painter.resetMatrix();

    if (!canPaint(band) )
	newPage();

//    if (band->layoutType() == BandInterface::LayoutBottom)
//	m_painter.translate( band->x(), freeSpace.bottom() - band->geometry().height() - band->indentationBottom());
//    else
//	if (band->layoutType() == BandInterface::LayoutTop)
//	    m_painter.translate( band->x(), freeSpace.top() + band->indentationTop());
//    else
//	if (band->layoutType() == BandInterface::LayoutFree)
//	    m_painter.translate(band->geometry().x(), band->geometry().y());
//
//    if (band->layoutType()== BandInterface::LayoutBottom)
//	freeSpace.setBottom( freeSpace.bottom() - band->geometry().height() - band->indentationTop() - band->indentationBottom() );
//    else
//	freeSpace.setTop( freeSpace.top() + band->geometry().height() + band->indentationTop() + band->indentationBottom() );
//
//    band->prPaint(&m_painter, QPointF(0, 0), QRectF(0, 0, m_currentPage->geometry().width(), m_currentPage->geometry().height()));

    QPointF translate;

    if (band->layoutType() == BandInterface::LayoutBottom)
	translate = QPointF( band->x(), freeSpace.bottom() - band->geometry().height() - band->indentationBottom());
    else
	if (band->layoutType() == BandInterface::LayoutTop)
	    translate = QPointF( band->x(), freeSpace.top() + band->indentationTop());
    else
	if (band->layoutType() == BandInterface::LayoutFree)
	    translate = QPointF(band->geometry().x(), band->geometry().y());

    if (band->layoutType()== BandInterface::LayoutBottom)
	freeSpace.setBottom( freeSpace.bottom() - band->geometry().height() - band->indentationTop() - band->indentationBottom() );
    else
	freeSpace.setTop( freeSpace.top() + band->geometry().height() + band->indentationTop() + band->indentationBottom() );

    band->prPaint(&m_painter, translate, QRectF(0, 0, m_currentPage->geometry().width(), m_currentPage->geometry().height()));




    m_painter.restore();
    if (!bandDone.contains(band))
	bandDone.append(band);

    m_lastProcessedBand = band;

    emit processBandAfter(band);
}


void PaintInterface::newPage()
{
    emit newPageBefore();

    postprocessCurrentPage();
    m_currentPageNumber++;
    m_report->m_scriptEngine->globalObject().setProperty("_page_", QScriptValue(m_report->m_scriptEngine, m_currentPageNumber), QScriptValue::ReadOnly);
    m_report->m_scriptEngine->globalObject().setProperty("_rpage_", QScriptValue(m_report->m_scriptEngine, m_report->m_scriptEngine->globalObject().property("_rpage_").toInteger() + 1), QScriptValue::ReadOnly);
    m_printer->newPage();
    freeSpace = m_currentPage->geometry();
    prepareCurrentPage();

    emit newPageAfter();
}


bool PaintInterface::canPaint(BandInterface * band)
{
    return (freeSpace.top() + band->geometry().height() <= freeSpace.bottom());
}


void PaintInterface::prepareCurrentPage()
{
    qDebug("PaintInterface::prepareCurrentPage()");
    emit showProcess(tr("Prepare page: %1").arg(m_currentPageNumber));

    freeSpace = m_currentPage->geometry();

    foreach(BandInterface * band, listFree)	    //process listFree first if it want paint on background
    {
	if (band->prNewPage())
	    processBand(band);
    }
    foreach(BandInterface * band, listTop)
    {
	if (band->prNewPage())
	    processBand(band);
    }
    for (int i = listBottom.count()-1; i>=0 ;i--)
    {
	BandInterface * band = listBottom.at(i);
	if (band->prNewPage())
	    processBand(band);
    }

}

void PaintInterface::postprocessCurrentPage()
{
    emit closePageBefore();

    foreach(BandInterface * band, listTop)
	if (band->prClosePage())
	    processBand(band);

    for (int i = listBottom.count()-1; i>=0 ;i--)
    {
	BandInterface * band = listBottom.at(i);
	if (band->prClosePage())
	    processBand(band);
    }

    foreach(BandInterface * band, listFree)  //process list free last if it paint on foreground
	if (band->prClosePage())
	    processBand(band);

    emit closePageAfter();
}


void PaintInterface::processDataset(DataSet * dtst)
{
    qDebug("PaintInterface::processDataset = %s", qPrintable(dtst->objectName()));
    emit processDatasetBefore(dtst);

    /// store dynmic data
    int currentDatasetRow = m_currentDatasetRow;
    int currentLineNumber = m_currentLineNumber;
    DataSet * currentDataset = m_currentDataset;

//    bool skipIteration = false;

    if (!dtst->isPopulated())
	dtst->populate();

    if (!dtst->first())
    {
	qDebug("setFirst error");
//	if ( !dtst->populate() || !dtst->first() );
//	    qDebug("dataset \'%s\' execution error: %s", dtst->objectName(), dtst->lastError());
    }

    if (!dtst->size())
    {
	qDebug("dataset is empty");
	return;
    }

    /*
    QDomElement dtstElement = m_report->m_doc.createElement("dataset");
    dtstElement.setAttribute("name", dtst->objectName());
    QDomElement rowElement = m_report->m_doc.createElement("row");
    for (int i = 0;i < dtst->size() ;i++)
    {
	QDomElement field = m_report->m_doc.createElement("field");
	field.setAttribute("type", "columnName");
	field.appendChild(m_report->m_doc.createTextNode(dtst->fieldName(i)));
	rowElement.appendChild(field);
    }
    dtstElement.appendChild(rowElement);
*/

    m_report->m_scriptEngine->globalObject().setProperty("_line_", QScriptValue(m_report->m_scriptEngine, 0), QScriptValue::ReadOnly);

    m_currentDataset = dtst;
    m_currentDatasetRow = -1;
    m_currentLineNumber = -1;

    /// making item group for dataset iteration
    BandList  currentGroup;
    foreach(BandInterface * band, listTop)
	if (band->dataset() == dtst->objectName() /*|| childrenDatasets.contains( band->dataset() )*/ )
	    currentGroup.append(band);

    do
    {
	m_currentDatasetRow++;
	m_currentLineNumber++;
	m_report->m_scriptEngine->globalObject().setProperty("_line_", QScriptValue(m_report->m_scriptEngine, m_currentLineNumber), QScriptValue::ReadOnly);

	foreach(BandInterface * band, currentGroup)
	{
	    qDebug("next in group = %s", qPrintable(band->objectName()));

		if (band->prData())
		    processBand(band);
	}

	//m_report->exportRecord(dtst->record(), dtstElement);
    }
    while (dtst->next());

//    m_report->m_exportNode.appendChild(dtstElement);

    foreach (BandInterface * band, currentGroup)
	if (!bandDone.contains(band))
	    bandDone.append(band);

    /// restore dynamic data
    m_currentDatasetRow = currentDatasetRow;
    m_currentLineNumber = currentLineNumber;
    m_currentDataset = currentDataset;

    emit processDatasetAfter(dtst);
}

int PaintInterface::currentPageNumber()
{
    return m_currentPageNumber;
}

int PaintInterface::currentDatasetRow()
{
    return m_currentDatasetRow;
}

int PaintInterface::currentDetailNumber()
{
    return m_currentLineNumber;
}

void PaintInterface::setDetailNumber(int num)
{
    m_currentLineNumber = num;
    m_report->m_scriptEngine->globalObject().setProperty("_line_", QScriptValue(m_report->m_scriptEngine, m_currentLineNumber), QScriptValue::ReadOnly);
}

QString PaintInterface::currentDatasetName()
{
    return m_currentDataset ? m_currentDataset->name() : QString();
}

Report::DataSet * PaintInterface::currentDataset()
{
    return m_currentDataset;
}

QRectF PaintInterface::pageFreeSpace()
{
    return freeSpace;
}

void PaintInterface::setPageFreeSpace (QRectF rect)
{
    freeSpace = rect;
}

BandInterface * PaintInterface::lastProcessedBand()
{
    return m_lastProcessedBand;
}
