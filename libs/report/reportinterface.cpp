/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 *   Copyright (C) 2009 by Mikhalov Alexander                              *
 *   alexmi3@rambler.ru                                                    *
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

#include "reportinterface.h"
#include "pageinterface.h"

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
#include <QTemporaryFile>
#include <QDir>
#include <QTimer>

#include "paintdevice.h"
#include "previewdialog.h"
#include "globals.h"
#include "paintinterface.h"
#include "bandinterface.h"
#include "dataset.h"

#define TIMER_DELAY 2000

static QScriptValue getSetDateFormat(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() != 2)
		return engine->undefinedValue();

	return QScriptValue(engine, context->argument(0).toDateTime().toString(context->argument(1).toString()));
}

static QScriptValue abs(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() != 2)
		return engine->undefinedValue();

	return QScriptValue(engine, context->argument(0).toDateTime().toString(context->argument(1).toString()));
}
/// abs arctan cos exp frac int ln pi round sin trunk
/// chr(int) compare (str, str), copy delete insert lengh lowercase namecase ord pos setlength trim uppercase
///dec inc inputBox messageBox random varType
//date dayof = 1..31 dayOfWeek daysInMonth monthOf now = date and time, time = curtime yearOf isLeapYear
namespace Report
{

ReportInterface::ReportInterface(QObject *parent)
		: QObject(parent), /*m_printer(0),*/ m_scriptEngine(0),m_sqlDatabase(QSqlDatabase::database())
{
    m_reportCanceled = false;	
	m_showPrintDialog = true;
	m_showPrintPreview = true;
	m_showExitConfirm = false;

    paintInterface = 0;
    m_version = 0.0;

    processDialog = new ProcessDialog();
    processDialog->setModal(true);
    connect(this, SIGNAL(showProcess(QString)), processDialog, SLOT(showProcess(QString)));

    processDialogTimer = new QTimer(this);
    processDialogTimer->setSingleShot( true );
    connect(processDialogTimer, SIGNAL(timeout()), this, SLOT(timedShowProcess()));
}

ReportInterface::~ReportInterface()
{
    delete processDialog;

    if (paintInterface)
	delete paintInterface;
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

	foreach(QString key, m_functionValues.keys())
		m_scriptEngine->globalObject().setProperty(key, m_scriptEngine->newFunction(m_functionValues[key].function, m_functionValues[key].args), m_functionValues[key].flags);

	foreach(QString key, m_values.keys())
		m_scriptEngine->globalObject().setProperty(key, m_scriptEngine->newVariant(m_values[key].value), m_values[key].flags);
}

void ReportInterface::scriptException(const QScriptValue & exception )
{
	QMessageBox::critical(0,tr("Uncaught exception at line %1").arg(exception.engine()->uncaughtExceptionLineNumber()), exception.toString());
}

bool ReportInterface::exec()
{
    m_reportCanceled=false;

    processDialogTimer->start( TIMER_DELAY );

    m_scriptEngine = new QScriptEngine(this);
    setReportFunction("dateFormat", ::getSetDateFormat,2);

    foreach(QString extention, m_scriptEngine->availableExtensions())
	if (!m_scriptEngine->importedExtensions().contains(extention))
	{
	    emit showProcess(tr("Importing extension: %1").arg(extention));
	    qApp->processEvents();
	    m_scriptEngine->importExtension(extention);
	}

    // prepare datasets
    foreach(Report::DataSet * dtst, datasets())
    {
	m_scriptEngine->globalObject().setProperty(dtst->objectName(), m_scriptEngine->newQObject(dtst), QScriptValue::ReadOnly);
	qApp->processEvents();
    }

    //prepare uis
    QUiLoader loader;
    foreach(QString path, m_uiPluginsPaths)
    {
	loader.addPluginPath( path );
	qApp->processEvents();
    }

    foreach(QString key, m_uis.keys())
    {
		QBuffer buf(this);
		buf.setData(m_uis[key].toByteArray());
		buf.open(QBuffer::ReadOnly);
		QWidget *widget = loader.load(&buf);
		widget->setObjectName( key );
		if (widget)
			m_scriptEngine->globalObject().setProperty(widget->objectName(), m_scriptEngine->newQObject(widget), QScriptValue::ReadOnly);
		else
			qDebug("ERROR while making UI!!!");
		qApp->processEvents();
    }

    setScriptEngineGlobalVariables();

    connect(m_scriptEngine, SIGNAL(signalHandlerException(QScriptValue)), SLOT(scriptException(QScriptValue)));

    QString script = this->script();
    foreach (QString var, this->scriptVars().keys())
	script.replace( "$" + var + "$", var);

    m_scriptEngine->evaluate(script);
    if (m_scriptEngine->hasUncaughtException())
	QMessageBox::critical(0,tr("Uncaught exception at line %1").arg(m_scriptEngine->uncaughtExceptionLineNumber()), m_scriptEngine->uncaughtException().toString());

    emit beforeExec();

    if (paintInterface) delete paintInterface;
    paintInterface = new PaintInterface(this);

    connect(paintInterface, SIGNAL(finished ()), this, SLOT(previewFinished()));
    connect(paintInterface, SIGNAL(showProcess(QString)), processDialog, SLOT(showProcess(QString)));

    m_scriptEngine->globalObject().setProperty("report", m_scriptEngine->newQObject(paintInterface), QScriptValue::ReadOnly);

    paintInterface->start();
}

void ReportInterface::timedShowProcess()
{
    processDialog->show();
    qApp->processEvents();
}

void ReportInterface::previewFinished()
{
    processDialogTimer->stop();
    processDialog->hide();

    emit afterExec();

    cleanUpObjects();
    if (!m_reportCanceled)
    {
	PreviewDialog d;
	d.setPrinterName(m_printerName);
	d.setShowPrintDialog(m_showPrintDialog);
	d.setShowExitConfirm(m_showExitConfirm);
	d.setToolButtonStyle( Qt::ToolButtonIconOnly );
	d.setIconSize(24);
	d.setDocument(pdf_file);
	d.setExportDocument(m_exportNode);
	emit beforePreviewShow(&d);
	d.setReportName(name());
	if (m_showPrintPreview)
		d.exec();
	else
		d.print();
    }
    m_doc.clear();

    delete paintInterface;
    paintInterface = 0;

    delete m_scriptEngine;
    m_scriptEngine=0;
//    return !m_reportCanceled;
}


void ReportInterface::cleanUpObjects()
{
	foreach(QObject * obj, children())
/*		if (dynamic_cast<SqlQuery*>(obj))
			delete dynamic_cast<SqlQuery*>(obj);
		else*/
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

QList< DataSet* > ReportInterface::datasets()
{
	return findChildren<DataSet*>();
}

/*
QVariantMap ReportInterface::queries()
{
	return m_queries;
}
*/
void ReportInterface::setDatasets(QList< Report::DataSet* > datasets)
{
	foreach(Report::DataSet* dtst, datasets)
		dtst->setParent(this);
}

/*
void ReportInterface::setQueries(QVariantMap queries)
{
	m_queries = queries;
}
*/

void ReportInterface::addDataset(DataSet * dataset)
{
	dataset->setParent(this);
}

QVariantMap ReportInterface::uis()
{
	return m_uis;
}

void ReportInterface::setUis(QVariantMap uis)
{
	m_uis = uis;
}

#warning FIXMI: not imlemented report file version checking
double ReportInterface::version()
{
    return m_version;
}

void ReportInterface::setVersion(double tVersion)
{
    m_version = tVersion;
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

void ReportInterface::setUiPluginsPaths(const QStringList & uiPluginsPaths)
{
	m_uiPluginsPaths=uiPluginsPaths;
}

void ReportInterface::setReportGlobalValue(QString name, QVariant value, const QScriptValue::PropertyFlags & flags) 
{
	ReportValue rv;
	rv.value=value;
	rv.flags=flags;
	m_values[name]=rv;
}

ReportInterface::ReportValues * ReportInterface::reportGlobalValues()
{
    return & m_values;
}

void ReportInterface::setReportFunction(const QString & name, const QScriptEngine::FunctionSignature & function, int args, const QScriptValue::PropertyFlags & flags) 
{
	FunctionValue fv;
	fv.function=function;
	fv.args=args;
	fv.flags=flags;
	m_functionValues[name]=fv;
}

QVariantMap ReportInterface::scriptVars()
{
   return m_scriptVars;
}

void ReportInterface::setScriptVars(QVariantMap vars)
{
    m_scriptVars = vars;
}

QString ReportInterface::printerName()
{
	return m_printerName;
}

void ReportInterface::setPrinterName(const QString & name)
{
	m_printerName = name;
}

bool ReportInterface::showPrintDialog()
{
	return m_showPrintDialog;
}

void ReportInterface::setShowPrintDialog(bool show)
{
	m_showPrintDialog = show;
}

bool ReportInterface::showPrintPreview()
{
	return m_showPrintPreview;
}

void ReportInterface::setShowPrintPreview(bool show)
{
	m_showPrintPreview = show;
}

bool ReportInterface::showExitConfirm()
{
	return m_showExitConfirm;
}

void ReportInterface::setShowExitConfirm(bool show)
{
	m_showExitConfirm = show;
}

}
