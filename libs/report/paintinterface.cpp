#include "paintinterface.h"
#include <QtSql>
#include <paintdevice.h>
//#include <sqlquery.h>
#include "reportinterface.h"
#include "pageinterface.h"


using namespace Report;

PaintInterface::PaintInterface(ReportInterface * report, QObject * parent)
	:QThread(parent)
{
    m_report = report;
    m_currentDataset = 0;
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
    foreach (PageInterface* page, m_report->findChildren<PageInterface *>())
    {
	if (!first)
	    m_printer->newPage();
	m_printer->setPaperSize(page->paperRect().size());
	m_printer->setPaperOrientation((QPrinter::Orientation)page->orientation());
	m_currentPage = page;
	processPage();
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
	    else    /// process dataset
	    {
		DataSet * dtst = m_report->findChild<DataSet *>(band->dataset());
		if (!dtst)
		    finish(tr("Dataset named \'%1\' not found for band \'%2\'").arg(dtst->objectName()).arg(band->objectName()) );

		processDataset(dtst);
	    }
	}
    }

    postprocessCurrentPage();
}

void PaintInterface::finish(QString error)
{
//    qDebug("PaintInterface::finish");
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

	m_painter.save();	

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

	band->prPaint(&m_painter, QPointF(0, 0), QRectF(0, 0, m_currentPage->geometry().width(), m_currentPage->geometry().height()));

	if (band->layoutType()== BandInterface::LayoutBottom)
	    freeSpace.setBottom( freeSpace.bottom() - band->geometry().height() -  band->indentation());
	else
	    freeSpace.setTop( freeSpace.top() + band->geometry().height() + band->indentation());

	m_painter.restore();

	///* test block
//	BandList joinedBands;
//	foreach (BandInterface * tBand, m_report->findChild<BandInterface *>() )
//	    if (tBand->joinTo() == band->objectName())
//		joinedBands.append( tBand);
	foreach(BandInterface * tBand, m_report->findChildren<BandInterface *>())
	{
	    if (tBand->joinTo() != band->objectName()) continue;
	    //if (!bandDone.contains(tBand))
	    {
		if (tBand->dataset().isEmpty())
		{
		    if ( tBand->prData() )
			processBand(tBand);
		}
		else    /// process dataset
		{
		    DataSet * dtst = m_report->findChild<DataSet *>(tBand->dataset());
		    if (!dtst)
			finish(tr("Dataset named \'%1\' not found for band \'%2\'").arg(dtst->objectName()).arg(tBand->objectName()) );

		    processDataset(dtst);
		}
	    }
	}
	///* end test block


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
    qDebug("PaintInterface::prepareCurrentPage()");
    emit showProcess(tr("Prepare page: %1").arg(m_currentPageNumber));

    freeSpace = m_currentPage->geometry();

    foreach(BandInterface * band, listFree)	    //process listFree first if it want paint on background
    {
	qDebug("new page for = %s", qPrintable(band->objectName()));
	if (band->prNewPage())
	    processBand(band);
    }
    foreach(BandInterface * band, listTop)
    {
	qDebug("new page for = %s", qPrintable(band->objectName()));
	if (band->prNewPage())
	    processBand(band);
    }
    for (int i = listBottom.count()-1; i>=0 ;i--)
    {
	BandInterface * band = listBottom.at(i);
	qDebug("new page for = %s", qPrintable(band->objectName()));
	if (band->prNewPage())
	    processBand(band);
    }

}

void PaintInterface::postprocessCurrentPage()
{
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

}

void PaintInterface::processDataset(DataSet * dtst)
{
    qDebug("PaintInterface::processDataset = %s", qPrintable(dtst->objectName()));
//    DataSet * dtst = m_report->findChild<DataSet *>(datasetName);
//    if (!dtst)
//    {
//	QString bandName = band ? band->objectName(): "Unknown";
//	finish(tr("Query named \'%1\' not found for band \'%2\'").arg(datasetName).arg(bandName) );
//    }

    /// store dynmic data
    int currentDatasetRow = m_currentDatasetRow;
    int currentLineNumber = m_currentLineNumber;
    DataSet * currentDataset = m_currentDataset;


    bool skipIteration = false;

    ///  lookup for children datasets
    QStringList childrenDatasets;
    foreach (DataSet* child, m_report->findChildren<DataSet *>())
	if (child->parentDataset() == dtst->objectName())
	    childrenDatasets.append(child->objectName());

    #warning //FIXME: check for already populated
    if ( !dtst->populate() )
	finish(tr("dataset \'%1\' execution error: %2").arg(dtst->objectName()).arg(dtst->lastError()));

    if ( !dtst->parentDataset().isEmpty() )
    {
	QString filter = dtst->filterCondition();
	DataSet* parentDataset = m_report->findChild<DataSet *>(dtst->parentDataset() );
	if (parentDataset)
	{

	    /// changing all '$field' including to parent field value
	    QString regExp ("\\$([\\w\\d]+)");
	    QRegExp rxlen(regExp);
	    int pos = 0;
	    while ((pos = rxlen.indexIn(filter, pos)) != -1)
	    {
		QString _fieldName = rxlen.cap(0);
		QString fieldName = rxlen.cap(1);
		filter.replace(_fieldName, parentDataset->value(fieldName).toString());
		pos += rxlen.matchedLength();
	    }
	    qDebug("filter = %s",qPrintable(filter));
	    qDebug("size before filter = %i",dtst->size());
	    if (filter.isEmpty())
		skipIteration = true;
	    else
		dtst->setFilter(dtst->filterColumn(), filter);
	    qDebug("size after filter = %i",dtst->size());
	}
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
    m_currentDatasetRow = 0;
    m_currentLineNumber = 0;

//    currentGroup.clear();

    /// making item group for dataset iteration
    BandList  currentGroup;
    foreach(BandInterface * band, listTop)
	if (band->dataset() == dtst->objectName() || childrenDatasets.contains( band->dataset() ) )
	    currentGroup.append(band);

    dtst->first();
    if (dtst->size() && !skipIteration)
    do
    {
	m_currentDatasetRow++;
	m_currentLineNumber++;
	m_report->m_scriptEngine->globalObject().setProperty("_line_", QScriptValue(m_report->m_scriptEngine, m_currentLineNumber), QScriptValue::ReadOnly);

	foreach(BandInterface * band, currentGroup)
	{
	    qDebug("next in group = %s", qPrintable(band->objectName()));
	    if (band->dataset() == dtst->objectName())
	    {
		if (band->prData())
		    processBand(band);
	    }
	    else
	    {
		processDataset (m_report->findChild<DataSet *>(band->dataset()));
	    }
	}

//	m_report->exportRecord(dtst->record(), dtstElement);
    }
    while (dtst->next());


//    m_report->m_exportNode.appendChild(dtstElement);


    foreach (BandInterface * band, currentGroup)
	if (!bandDone.contains(band))
	    bandDone.append(band);
    currentGroup.clear();

    /// restore dynamic data
    m_currentDatasetRow = currentDatasetRow;
    m_currentLineNumber = currentLineNumber;
    m_currentDataset = currentDataset;

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

QString PaintInterface::currentDatasetName()
{
    return m_currentDataset ? m_currentDataset->name() : QString();
}
