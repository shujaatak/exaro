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

#include "reportinterface.h"
#include "pageinterface.h"
#include "sqlquery.h"

#include <QBuffer>
#include <QWidget>
#include <QDate>
#include <QTime>
#include <QStyleOptionGraphicsItem>
#include <QSqlError>
#include <QCoreApplication>
#include <QPrintPreviewWidget>
#include <QSqlField>     
#include <QMessageBox>

#include "previewdialog.h"
#include "paintdevice.h"
#include "globals.h"

namespace Report
{

ReportInterface::ReportInterface(QObject *parent)
		: QObject(parent), m_printer(0), m_scriptEngine(0),m_sqlDatabase(QSqlDatabase::database())
{
	m_reportCanceled = false;
}

ReportInterface::~ReportInterface()
{
}


void ReportInterface::addOrderedBand(QList<BandInterface *> & m_bands, BandInterface * band)
{
	int sz = m_bands.size();
	for (int i = 0;i < m_bands.size();i++)
		if (m_bands[i]->order() > band->order())
		{
			m_bands.insert(i, band);
			break;
		}

	if (sz == m_bands.size())
		m_bands.push_back(band);
}

void ReportInterface::paintPage(PageInterface * page)
{
	if (!page || m_reportCanceled)
		return;


	m_currentPage = page;

	m_pageHeaderBands.clear();
	m_titleBands.clear();
	m_detailContainerBands.clear();
	m_summaryBands.clear();
	m_pageFooterBands.clear();
	m_pageOverlayBands.clear();

	m_scriptEngine->globalObject().setProperty("_pageNumber_", QScriptValue(m_scriptEngine, 1), QScriptValue::ReadOnly);

	foreach(QObject * obj, page->children())
	{
		BandInterface * band = dynamic_cast<BandInterface *>(obj);
		if (!band)
			continue;

		switch (band->bandType())
		{
			case BandInterface::PageHeader:
				addOrderedBand(m_pageHeaderBands, band);
				break;
			case BandInterface::Title:
				addOrderedBand(m_titleBands, band);
				break;
			case BandInterface::DetailContainer:
				addOrderedBand(m_detailContainerBands, band);
				break;
			case BandInterface::Summary:
				addOrderedBand(m_summaryBands, band);
				break;
			case BandInterface::PageFooter:
				addOrderedBand(m_pageFooterBands, band);
				break;
			case BandInterface::Overlay:
				m_pageOverlayBands.push_back(band);
			default:
				break;
		}
	}


	if (!m_detailContainerBands.size())
	{
		prepareCurrentPage();
		foreach(BandInterface * band, m_summaryBands)
			paintBand(band);
		paintOverlays();
		return;
	}

	foreach(BandInterface * detailContainerBand, m_detailContainerBands)
	{
		QList<BandInterface *> detailHeaders;
		QList<BandInterface *> details;
		QList<BandInterface *> detailFooters;
		prepareCurrentPage();

		m_scriptEngine->globalObject().setProperty("_detailNumber_", QScriptValue(m_scriptEngine, 0), QScriptValue::ReadOnly);

		foreach(QObject * obj, ((QObject*)detailContainerBand)->children())
		{
			BandInterface * bd = dynamic_cast<BandInterface *>(obj);
			if (!bd)
				continue;
			switch (bd->bandType())
			{
				case BandInterface::DetailHeader:
					addOrderedBand(detailHeaders, bd);
					break;
				case BandInterface::Detail:
					addOrderedBand(details, bd);
					break;
				case BandInterface::DetailFooter:
					addOrderedBand(detailFooters, bd);
					break;
				default:
					break;
			}
		}
		SqlQuery * detailQuery = detailContainerBand->findQuery(detailContainerBand->query());
		if (!detailQuery || !detailQuery->isActive())
		{
			// no query ?!?!?!?
			foreach(BandInterface * band, detailHeaders)
			{
				if (!canPaint(band))
					newPage();
				paintBand(band);
			}
			foreach(BandInterface * band, details)
			{
				if (!canPaint(band))
					newPage();
				paintBand(band);
			}

			foreach(BandInterface * band, detailFooters)
			{
				if (!canPaint(band))
					newPage();
				paintBand(band);
			}
			continue;
		}

		if (!detailQuery->isValid())
			detailQuery->first();

		QDomElement qryElement = m_doc.createElement("query");
		qryElement.setAttribute("name", detailQuery->objectName());
		QDomElement rowElement = m_doc.createElement("row");
		for (int i = 0;i < detailQuery->record().count();i++)
		{
			QDomElement field = m_doc.createElement("field");
			field.setAttribute("type", "columnName");
			field.appendChild(m_doc.createTextNode(detailQuery->record().fieldName(i)));
			rowElement.appendChild(field);
		}
		qryElement.appendChild(rowElement);

		bool first=true;
		do
		{
			foreach(BandInterface * band, detailFooters)
			{
				if (!band->groupFieldValue().isValid())
					band->setGroupFieldValue(detailContainerBand->queryField(detailContainerBand->query(), band->groupField()));

				if (band->groupFieldValue() != detailContainerBand->queryField(detailContainerBand->query(), band->groupField()))
				{
					if (!canPaint(band))
						newPage();
					paintBand(band);
					band->setGroupFieldValue(detailContainerBand->queryField(detailContainerBand->query(), band->groupField()));
				}
			}

			foreach(BandInterface * band, detailHeaders)
				if (band->groupFieldValue() != detailContainerBand->queryField(detailContainerBand->query(), band->groupField()))
				{
					if (!canPaint(band) || (!first && band->forceNewPage()))
						newPage();
					paintBand(band);
					band->setGroupFieldValue(detailContainerBand->queryField(detailContainerBand->query(), band->groupField()));
					if (band->resetDetailNumber())
						m_scriptEngine->globalObject().setProperty("_detailNumber_", QScriptValue(m_scriptEngine, 0), QScriptValue::ReadOnly);
				}

			m_scriptEngine->globalObject().setProperty("_detailNumber_", QScriptValue(m_scriptEngine, m_scriptEngine->globalObject().property("_detailNumber_").toInteger() + 1), QScriptValue::ReadOnly);

			foreach(BandInterface * band, details)
			{
				if (!canPaint(band))
				{
					newPage();
					foreach(BandInterface * band, detailHeaders)
						if (band->reprintOnNewPage())
							paintBand(band);
				}
				paintBand(band);
			}
			exportRecord(detailQuery->record(), qryElement);
			first=false;
		}
		while (detailQuery->next());

		m_exportNode.appendChild(qryElement);

		foreach(BandInterface * band, detailFooters)
		{
			if (!canPaint(band))
				newPage();
			paintBand(band);
		}

		foreach(BandInterface * band, detailFooters)
			band->setGroupFieldValue(QVariant());

		foreach(BandInterface * band, detailHeaders)
			band->setGroupFieldValue(QVariant());
	}
	foreach(BandInterface * band, m_summaryBands)
	{
		if (!canPaint(band))
			newPage();
		paintBand(band);
	}
	paintOverlays();
}


void ReportInterface::newPage()
{
	paintOverlays();
	m_scriptEngine->globalObject().setProperty("_pageNumber_", QScriptValue(m_scriptEngine, m_scriptEngine->globalObject().property("_pageNumber_").toInteger() + 1), QScriptValue::ReadOnly);
	m_scriptEngine->globalObject().setProperty("_reportPageNumber_", QScriptValue(m_scriptEngine, m_scriptEngine->globalObject().property("_reportPageNumber_").toInteger() + 1), QScriptValue::ReadOnly);
	m_printer->newPage();
	prepareCurrentPage();
}

bool ReportInterface::canPaint(BandInterface * band)
{
	return (m_currentTop + band->geometry().height() <= m_currentBottom);
}

void ReportInterface::prepareCurrentPage()
{
	m_currentTop = m_currentPage->geometry().y();
	m_currentBottom = m_currentPage->geometry().bottom();

	foreach(BandInterface * band, m_pageHeaderBands)
		paintBand(band);

	if (m_scriptEngine->globalObject().property("_pageNumber_").toInteger() == 1)
		foreach(BandInterface * band, m_titleBands)
			paintBand(band);

	foreach(BandInterface * band, m_pageFooterBands)
		paintBand(band);
}

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

void ReportInterface::paintObjects(ItemInterface * item, QPointF translate, const QRectF & clipRect)
{
	if (!item)
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

void ReportInterface::paintBand(BandInterface * band)
{
	if (!band || m_reportCanceled)
		return;

	m_painter.save();

	if (band->bandType() == BandInterface::PageFooter)
		m_painter.translate(m_currentPage->geometry().x(), m_currentBottom - band->geometry().height());
	else
		m_painter.translate(m_currentPage->geometry().x(), m_currentTop);

	m_currentHeight = 0;

	paintObjects(band, QPointF(0, 0), QRectF(0, 0, m_currentPage->geometry().width(), m_currentPage->geometry().height()));

	m_currentHeight += band->indentation();

	if (band->bandType() == BandInterface::PageFooter)
		m_currentBottom -= band->geometry().height() + m_currentHeight;
	else
		m_currentTop += band->geometry().height() + m_currentHeight;

	m_painter.restore();
}

void ReportInterface::paintOverlays()
{
	if (m_reportCanceled)
		return;

	foreach(BandInterface * band, m_pageOverlayBands)
	{
		m_painter.save();
		m_painter.translate(band->geometry().x(), band->geometry().y());
		paintObjects(band, QPointF(0, 0), QRectF(0, 0, m_currentPage->geometry().width(), m_currentPage->geometry().height()));
		m_painter.restore();
	}
}

void ReportInterface::setReportObjectsToScriptEngineGlobalVariables(QObject * object)
{
	m_scriptEngine->globalObject().setProperty(object->objectName(), m_scriptEngine->newQObject(object), QScriptValue::ReadOnly);

	foreach(QObject * obj, object->children())
		setReportObjectsToScriptEngineGlobalVariables(obj);
}

void ReportInterface::setScriptEngineGlobalVariables()
{
	setReportObjectsToScriptEngineGlobalVariables(this);

	m_scriptEngine->globalObject().setProperty("_reportPageNumber_", QScriptValue(m_scriptEngine, 1), QScriptValue::ReadOnly);
	m_scriptEngine->globalObject().setProperty("_pageNumber_", QScriptValue(m_scriptEngine, 1), QScriptValue::ReadOnly);
	m_scriptEngine->globalObject().setProperty("_detailNumber_", QScriptValue(m_scriptEngine, 0), QScriptValue::ReadOnly);
	m_scriptEngine->globalObject().setProperty("_currentDate_", QScriptValue(m_scriptEngine, QDate::currentDate().toString()), QScriptValue::ReadOnly);
	m_scriptEngine->globalObject().setProperty("_currentTime_", QScriptValue(m_scriptEngine, QTime::currentTime().toString()), QScriptValue::ReadOnly);

	foreach(QObject * obj, m_objectList)
		m_scriptEngine->globalObject().setProperty(obj->objectName(), m_scriptEngine->newQObject(obj), QScriptValue::ReadOnly);
}

void ReportInterface::scriptException(const QScriptValue & exception )
{
	QMessageBox::critical(0,tr("Uncaught exception at line %1").arg(exception.engine()->uncaughtExceptionLineNumber()), exception.toString());
}

bool ReportInterface::exec()
{
	m_reportCanceled=false;
	m_splashScreen.setPixmap(QPixmap(QString(":/images/designer%1.png").arg(1 + qrand() % 5)));
	m_splashScreen.show();
	qApp->processEvents();
	m_scriptEngine = new QScriptEngine(this);
	foreach(QString extention, m_scriptEngine->availableExtensions())
		if (!m_scriptEngine->importedExtensions().contains(extention))
		{
			m_splashScreen.showMessage(tr("Importing extension: %1").arg(extention));
			m_scriptEngine->importExtension(extention);
		}


	// prepare queries
	foreach(QString key, m_queries.keys())
	{
		SqlQuery * sql = new SqlQuery(this,m_sqlDatabase);
		sql->setObjectName(key);
		sql->prepare(m_queries[key].toString());
		m_scriptEngine->globalObject().setProperty(key, m_scriptEngine->newQObject(sql), QScriptValue::ReadOnly);
		qApp->processEvents();
	}

	//prepare uis
	QUiLoader loader;
	foreach(QString key, m_uis.keys())
	{
		QBuffer buf(this);
		buf.setData(m_uis[key].toByteArray());
		buf.open(QBuffer::ReadOnly);
		QWidget *widget = loader.load(&buf);
		if (widget)
			m_scriptEngine->globalObject().setProperty(widget->objectName(), m_scriptEngine->newQObject(widget), QScriptValue::ReadOnly);
	}

	setScriptEngineGlobalVariables();

	connect(m_scriptEngine, SIGNAL(signalHandlerException(QScriptValue)), SLOT(scriptException(QScriptValue)));

	m_scriptEngine->evaluate(m_script);
	if (m_scriptEngine->hasUncaughtException())
		QMessageBox::critical(0,tr("Uncaught exception at line %1").arg(m_scriptEngine->uncaughtExceptionLineNumber()), m_scriptEngine->uncaughtException().toString());



	emit beforeExec();

	m_doc.appendChild(m_doc.createComment("Author '" + author() + "'"));

	if (!m_reportCanceled)
	{
		m_printer = new PaintDevice;
		m_printer->setOutputDocument(&m_doc);
		QDomNode report = m_doc.createElement("report");
		m_printer->setReportRoot(report);
		m_exportNode = m_doc.createElement("export");

		m_painter.begin(m_printer);
		m_painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
		foreach(QObject * obj, children())
		{
			m_printer->newPage();
			if (dynamic_cast<PageInterface*>(obj))
			{
				m_printer->setPaperSize((QPrinter::PaperSize)dynamic_cast<PageInterface*>(obj)->pageSize());
				m_printer->setPaperOrientation((QPrinter::Orientation)dynamic_cast<PageInterface*>(obj)->orientation());
				paintPage(dynamic_cast<PageInterface*>(obj));
			}
		}
		m_painter.end();
		delete m_printer;
		m_printer = 0;
		m_doc.appendChild(report);
		m_doc.appendChild(m_exportNode);
	}
	emit afterExec();

	cleanUpObjects();
	if (!m_reportCanceled)
	{
		PreviewDialog d;
		d.setDocumentNode(m_doc.documentElement());
		m_splashScreen.finish(&d);
		d.exec();
	}
	else
		m_splashScreen.finish(0);
	m_doc.clear();
	delete m_scriptEngine;
	m_scriptEngine=0;
	return !m_reportCanceled;
}

void ReportInterface::cleanUpObjects()
{
	foreach(QObject * obj, children())
		if (dynamic_cast<SqlQuery*>(obj))
			delete dynamic_cast<SqlQuery*>(obj);
		else
			delete dynamic_cast<QWidget*>(obj);
}

bool ReportInterface::canContain(QObject * object)
{
	return (bool)(dynamic_cast<PageInterface*>(object));
}

QString ReportInterface::script()
{
	return m_script;
}

void ReportInterface::setScript(const QString & script)
{
	m_script = script;
}

QString ReportInterface::name()
{
	return m_name;
}

void ReportInterface::setName(const QString & name)
{
	m_name = name;
}

QString ReportInterface::author()
{
	return m_author;
}

void ReportInterface::cancelReport()
{
	m_reportCanceled = true;
}

void ReportInterface::setAuthor(const QString & author)
{
	m_author = author;
}

QVariantMap ReportInterface::queries()
{
	return m_queries;
}

void ReportInterface::setQueries(QVariantMap queries)
{
	m_queries = queries;
}

QVariantMap ReportInterface::uis()
{
	return m_uis;
}

void ReportInterface::setUis(QVariantMap uis)
{
	m_uis = uis;
}

QScriptEngine * ReportInterface::scriptEngine()
{
	return m_scriptEngine;
}

const QObjectList & ReportInterface::globalObjects()
{
	return m_objectList;
}

void ReportInterface::setGlobalObjects(const QObjectList & objectList)
{
	m_objectList=objectList;
}

void ReportInterface::setDatabase(const QSqlDatabase & db)
{
	m_sqlDatabase=db;
}

}
