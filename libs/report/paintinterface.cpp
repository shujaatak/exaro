#include "paintinterface.h"

#include "reportinterface.h"
#include "pageinterface.h"

using namespace Report;

PaintInterface::PaintInterface(ReportInterface * report, QObject * parent)
	:QObject(parent)
{
    /*
    m_page = ;
    m_scriptEngine = scriptEngine;
    m_painter = painter;
*/
    m_report = report;
    LayoutManager::splitOnLayoutTypesSorted(report->m_currentPage, &listTop, &listBottom, &listFree);

}


void PaintInterface::run()
{
    initItem();

    freeSpace = m_report->m_currentPage->geometry();
    this->currentPageNumber = 1;

    prepareCurrentPage();

    foreach(BandInterface * band, listTop)
	if (band->accommodationType() == band->AccomodationOnce)
	    paintBand(band);

}

void PaintInterface::initItem()
{

    foreach(BandInterface * band, listTop)
	if (!band->printingPrepare(this))
	    showError(QString("Error in item \'%1\' \n%2").arg(band->objectName()).arg(band->lastError()));
    foreach(BandInterface * band, listBottom)
	if (!band->printingPrepare(this))
	    showError(QString("Error in item \'%1\' \n%2").arg(band->objectName()).arg(band->lastError()));
    foreach(BandInterface * band, listFree)
	if (!band->printingPrepare(this))
	    showError(QString("Error in item \'%1\' \n%2").arg(band->objectName()).arg(band->lastError()));

}

void PaintInterface::showError(QString err)
{
    qDebug("Error:\n %s", qPrintable(err));
}

void PaintInterface::paintBand(BandInterface * band)
{

	if (!band /*|| m_reportCanceled*/)
		return;
	if (!band->isEnabled())
	    return;
	QPainter * m_painter = &m_report->m_painter;
	PageInterface * m_currentPage = m_report->m_currentPage;
	m_painter->save();
	band->prepare(m_painter);
	if (band->layoutType() == BandInterface::LayoutBottom)
		m_painter->translate(/*freeSpace.x() +*/ band->x(), freeSpace.bottom() - band->geometry().height());
	else
	    if (band->layoutType() == BandInterface::LayoutTop)
		m_painter->translate(/*freeSpace.x() +*/ band->x(), freeSpace.top());
	    else
		if (band->layoutType() == BandInterface::LayoutFree)
		    m_painter->translate(band->geometry().x(), band->geometry().y());

//	m_report->m_currentHeight = 0;
//	m_currentSize = QSize(0,0);

	paintObjects(band, QPointF(0, 0), QRectF(0, 0, m_currentPage->geometry().width(), m_currentPage->geometry().height()));

//	m_report->m_currentHeight += band->indentation();

	if (band->layoutType()== BandInterface::LayoutBottom)
		freeSpace.setBottom( freeSpace.bottom() - band->geometry().height() -  band->indentation());
	else
		freeSpace.setTop( freeSpace.top() + band->geometry().height() + band->indentation());
//	band->unstretch();
	m_painter->restore();
}

/*

  void ReportInterface::newPage()
{
	int pageNo=m_scriptEngine->globalObject().property("_pageNumber_").toInteger() +1;
	m_splashScreen.showMessage(tr("Prepare page: %1").arg(pageNo));
	paintOverlays();
	m_scriptEngine->globalObject().setProperty("_pageNumber_", QScriptValue(m_scriptEngine, pageNo), QScriptValue::ReadOnly);
	m_scriptEngine->globalObject().setProperty("_reportPageNumber_", QScriptValue(m_scriptEngine, m_scriptEngine->globalObject().property("_reportPageNumber_").toInteger() + 1), QScriptValue::ReadOnly);
	m_printer->newPage();
	prepareCurrentPage();
}

bool ReportInterface::canPaint(BandInterface * band)
{
	return (m_currentTop + band->geometry().height() <= m_currentBottom);
}
*/
void PaintInterface::prepareCurrentPage()
{
    m_report->m_currentTop = m_report->m_currentPage->geometry().y();
    m_report->m_currentBottom = m_report->m_currentPage->geometry().bottom();

    foreach(BandInterface * band, listTop)
	if (band->accommodationType() == band->AccomodationEveryPage || (band->accommodationType() == band->AccomodationFirstPage && currentPageNumber == 1))
	    paintBand(band);

//    foreach(BandInterface * band, listBottom)
//	if (band->accommodationType() == band->AccomodationEveryPage || (band->accommodationType() == band->AccomodationFirstPage && currentPageNumber == 1))
//	    paintBand(band);

    for (int i = listBottom.count()-1; i>=0 ;i--)
    {
	BandInterface * band = listBottom.at(i);
	if (band->accommodationType() == band->AccomodationEveryPage || (band->accommodationType() == band->AccomodationFirstPage && currentPageNumber == 1))
	    paintBand(band);
    }
}

void PaintInterface::postprocessCurrentPage()
{
    foreach(BandInterface * band, listFree)
	if (band->accommodationType() == band->AccomodationEveryPage || (band->accommodationType() == band->AccomodationFirstPage && currentPageNumber == 1))
	    paintBand(band);
}

/*
void ReportInterface::exportRecord(const QSqlRecord & record, QDomElement & el)
{
	QDomElement rowElement = m_doc.createElement("row");
	for (int i = 0;i < record.count();i++)
	{
		QDomElement field = m_doc.createElement("field");
		field.setAttribute("type", typeIsNumber(record.field(i).value().type()) ? QString("float") : QString("string"));
		field.appendChild(m_doc.createTextNode(record.field(i).value().toString()));
		rowElement.appendChild(field);
	}
	el.appendChild(rowElement);
}
*/

void PaintInterface::paintObjects(ItemInterface * item, QPointF translate, const QRectF & clipRect)
{
	if (!item || !item->isEnabled())
		return;

	QStyleOptionGraphicsItem option;
	option.type = 31;
	option.exposedRect = dynamic_cast<BandInterface *>(item) ? QRectF(0, 0, dynamic_cast<BandInterface *>(item)->geometry().width(), dynamic_cast<BandInterface *>(item)->geometry().height()) : item->geometry();
	m_report->m_painter.save();
	option.exposedRect.translate(translate);
	m_report->m_painter.setClipRect(clipRect);
	item->paint(&m_report->m_painter, &option);
	m_report->m_painter.restore();
	translate += option.exposedRect.topLeft();
	foreach(QObject * obj, ((QObject*)item)->children())
		paintObjects(dynamic_cast<ItemInterface *>(obj), translate, option.exposedRect);
}

