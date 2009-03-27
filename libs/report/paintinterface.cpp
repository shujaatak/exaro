#include "paintinterface.h"
#include <QtSql>
#include <paintdevice.h>
#include <sqlquery.h>
#include "reportinterface.h"
#include "pageinterface.h"


using namespace Report;

PaintInterface::PaintInterface(ReportInterface * report, QObject * parent)
	:QThread(parent)
{
    m_report = report;
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
    bool first=true;
    foreach(QObject * obj, m_report->children())
    {
	if (!first)
	    m_printer->newPage();
	if (dynamic_cast<PageInterface*>(obj))
	{
	    m_printer->setPaperSize(dynamic_cast<PageInterface*>(obj)->paperRect().size());
	    m_printer->setPaperOrientation((QPrinter::Orientation)dynamic_cast<PageInterface*>(obj)->orientation());

	    m_currentPage = dynamic_cast<PageInterface*>(obj);
	    processPage();
	}
    }
    m_painter.end();
    delete m_printer;
    m_printer = 0;

}

void PaintInterface::processPage()
{
    listTop.clear();
    listBottom.clear();
    listFree.clear();

    LayoutManager::splitOnLayoutTypesSorted(m_currentPage, &listTop, &listBottom, &listFree);

    initBands();

    freeSpace = m_currentPage->geometry();
    this->m_currentPageNumber = 1;
    m_report->m_scriptEngine->globalObject().setProperty("_page_", QScriptValue(m_report->m_scriptEngine, 1), QScriptValue::ReadOnly);

    prepareCurrentPage();

    bandDone.clear();

    foreach(BandInterface * band, listTop)
	if (band->accommodationType() == band->AccomodationOnce && !bandDone.contains(band))
	    if (band->dataset().isEmpty())
		processBand(band, pmNormal);
    else
	processDataset(band->dataset(), band);

}

void PaintInterface::finish(QString error)
{
//    qDebug("PaintInterface::finish");
    showError(error);
    deleteLater () ;
}

void PaintInterface::initBands()
{
//    qDebug("PaintInterface::initBands()");

    foreach(BandInterface * band, listTop)
	if (!band->init(this))
	    showError(QString("Error in item \'%1\' \n%2").arg(band->objectName()).arg(band->lastError()));
	else
	    foreach(QObject * obj, band->QObject::children())
		if (dynamic_cast<Report::ItemInterface*>(obj))
		    dynamic_cast<Report::ItemInterface*>(obj)->init(this);
    foreach(BandInterface * band, listBottom)
	if (!band->init(this))
	    showError(QString("Error in item \'%1\' \n%2").arg(band->objectName()).arg(band->lastError()));
    	else
	    foreach(QObject * obj, band->QObject::children())
		if (dynamic_cast<Report::ItemInterface*>(obj))
		    dynamic_cast<Report::ItemInterface*>(obj)->init(this);
    foreach(BandInterface * band, listFree)
	if (!band->init(this))
	    showError(QString("Error in item \'%1\' \n%2").arg(band->objectName()).arg(band->lastError()));
    	else
	    foreach(QObject * obj, band->QObject::children())
		if (dynamic_cast<Report::ItemInterface*>(obj))
		    dynamic_cast<Report::ItemInterface*>(obj)->init(this);
}


void PaintInterface::showError(QString err)
{
    qDebug("Error:\n %s", qPrintable(err));
}

void PaintInterface::processBand(BandInterface * band, PrintMode pMode)
{
	if (!band /*|| m_reportCanceled*/)
		return;
//	qDebug("PaintInterface::processBand = %s", qPrintable(band->objectName()));
	if (!band->isEnabled())
	    return;

	m_painter.save();

	#warning    "FIXME - need optimization and proceed child item only if band be paint"
	foreach(QObject * obj, ((QObject*)band)->children())		//preProcess all child items
	{
//	    qDebug("process child name=%s",qPrintable(obj->objectName()));
	    ItemInterface * item = dynamic_cast<ItemInterface *>(obj);
	    if (item)
		if (!item->prePaint(&m_painter, pMode))
		    finish(item->lastError());
	}

	if (!band->prePaint(&m_painter, pMode))				//preProcess band
	{
	    m_painter.restore();
	    return;
	}

	if (!canPaint(band) )
	    newPage();

	if (band->layoutType() == BandInterface::LayoutBottom)
		m_painter.translate(/*freeSpace.x() +*/ band->x(), freeSpace.bottom() - band->geometry().height());
	else
	    if (band->layoutType() == BandInterface::LayoutTop)
		m_painter.translate(/*freeSpace.x() +*/ band->x(), freeSpace.top());
	    else
		if (band->layoutType() == BandInterface::LayoutFree)
		    m_painter.translate(band->geometry().x(), band->geometry().y());

//	m_report->m_currentHeight = 0;
//	m_currentSize = QSize(0,0);

	    ///NEED CHECK FOR GROUP

	paintObjects(band, QPointF(0, 0), QRectF(0, 0, m_currentPage->geometry().width(), m_currentPage->geometry().height()));

	foreach(QObject * obj, ((QObject*)band)->children())		//postProcess all child items
	{
	    ItemInterface * item = dynamic_cast<ItemInterface *>(obj);
	    if (item)
		item->postPaint();
	}

	band->postPaint();				//postProcess band

//	m_report->m_currentHeight += band->indentation();

	if (band->layoutType()== BandInterface::LayoutBottom)
		freeSpace.setBottom( freeSpace.bottom() - band->geometry().height() -  band->indentation());
	else
		freeSpace.setTop( freeSpace.top() + band->geometry().height() + band->indentation());
//	band->unstretch();
	m_painter.restore();
}



void PaintInterface::newPage()
{
    postprocessCurrentPage();
    m_currentPageNumber++;
    m_report->m_scriptEngine->globalObject().setProperty("_page_", QScriptValue(m_report->m_scriptEngine, m_currentPageNumber), QScriptValue::ReadOnly);
    m_report->m_scriptEngine->globalObject().setProperty("_rpage_", QScriptValue(m_report->m_scriptEngine, m_report->m_scriptEngine->globalObject().property("_rpage_").toInteger() + 1), QScriptValue::ReadOnly);
    m_printer->newPage();
    freeSpace = m_currentPage->geometry();
    prepareCurrentPage();

}


bool PaintInterface::canPaint(BandInterface * band)
{
    return (freeSpace.top() + band->geometry().height() <= freeSpace.bottom());
}


void PaintInterface::prepareCurrentPage()
{
    emit showProcess(tr("Prepare page: %1").arg(m_currentPageNumber));
    m_report->m_currentTop = m_currentPage->geometry().y();
    m_report->m_currentBottom = m_currentPage->geometry().bottom();

    foreach(BandInterface * band, listTop)
	if (band->accommodationType() == band->AccomodationEveryPage || (band->accommodationType() == band->AccomodationFirstPage && m_currentPageNumber == 1))
	    processBand(band);

    for (int i = listBottom.count()-1; i>=0 ;i--)
    {
	BandInterface * band = listBottom.at(i);
	if (band->accommodationType() == band->AccomodationEveryPage || (band->accommodationType() == band->AccomodationFirstPage && m_currentPageNumber == 1))
	    processBand(band);
    }

//    qDebug("size of  current group is %i", currentGroup.size());

    if (!currentGroup.isEmpty())
	foreach (BandInterface * band, currentGroup)
		processBand(band, pmNewPage);
}

void PaintInterface::postprocessCurrentPage()
{
    foreach(BandInterface * band, listFree)
	if (band->accommodationType() == band->AccomodationEveryPage || (band->accommodationType() == band->AccomodationFirstPage && m_currentPageNumber == 1))
	    processBand(band);
}


void PaintInterface::paintObjects(ItemInterface * item, QPointF translate, const QRectF & clipRect)
{
	if (!item || !item->isEnabled())
		return;

	QStyleOptionGraphicsItem option;
	option.type = 31;
	option.exposedRect = dynamic_cast<BandInterface *>(item) ? QRectF(0, 0, dynamic_cast<BandInterface *>(item)->geometry().width(), dynamic_cast<BandInterface *>(item)->geometry().height()) : item->geometry();
	m_painter.save();
	option.exposedRect.translate(translate);
	m_painter.setClipRect(clipRect);
	item->paint(&m_painter, &option);
	m_painter.restore();
	translate += option.exposedRect.topLeft();
	foreach(QObject * obj, ((QObject*)item)->children())
		paintObjects(dynamic_cast<ItemInterface *>(obj), translate, option.exposedRect);
}


void PaintInterface::processDataset(QString datasetName, BandInterface * band )
{
//    qDebug("PaintInterface::processDataset = %s", qPrintable(datasetName));
    DataSet * dtst = m_report->findChild<DataSet *>(datasetName);
    if (!dtst)
    {
	QString bandName = band ? band->objectName(): "Unknown";
	finish(tr("Query named \'%1\' not found for band \'%2\'").arg(datasetName).arg(bandName) );
    }

    //already exec()'d ?
    if (!dtst->first())
	if ( !(dtst->populate() && dtst->first()) )
	    finish(tr("query \'%1\' execution error: %2").arg(datasetName).arg(dtst->lastError().text()));

    QDomElement dtstElement = m_report->m_doc.createElement("dataset");
    dtstElement.setAttribute("name", dtst->objectName());
    QDomElement rowElement = m_report->m_doc.createElement("row");
    for (int i = 0;i < dtst->record().count();i++)
    {
	QDomElement field = m_report->m_doc.createElement("field");
	field.setAttribute("type", "columnName");
	field.appendChild(m_report->m_doc.createTextNode(dtst->record().fieldName(i)));
	rowElement.appendChild(field);
    }
    dtstElement.appendChild(rowElement);

    m_report->m_scriptEngine->globalObject().setProperty("_line_", QScriptValue(m_report->m_scriptEngine, 0), QScriptValue::ReadOnly);

    m_currentDataset = dtst;
    m_currentDatasetRow = 0;
    m_currentLineNumber = 0;

    currentGroup.clear();

    foreach(BandInterface * band, listTop)
	if (band->dataset() == datasetName)
	    currentGroup.append(band);

    do
    {
	m_currentDatasetRow++;
	m_currentLineNumber++;
	m_report->m_scriptEngine->globalObject().setProperty("_line_", QScriptValue(m_report->m_scriptEngine, m_currentLineNumber), QScriptValue::ReadOnly);

	foreach(BandInterface * band, currentGroup)
	    processBand(band);

	m_report->exportRecord(dtst->record(), dtstElement);
    }
    while (dtst->next());

    m_report->m_exportNode.appendChild(dtstElement);

    foreach (BandInterface * band, currentGroup)
	bandDone.append(band);
    currentGroup.clear();
}

int PaintInterface::currentPageNumber()
{
    return m_currentPageNumber;
}

int PaintInterface::currentDatasetRow()
{
    return m_currentDatasetRow;
}

void PaintInterface::setDetailNumber(int num)
{
    m_currentLineNumber = num;
    m_report->m_scriptEngine->globalObject().setProperty("_line_", QScriptValue(m_report->m_scriptEngine, m_currentLineNumber), QScriptValue::ReadOnly);
}

