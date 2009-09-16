#include "exaro.h"
#include <reportengine.h>
#include <previewdialog.h>

Exaro * Exaro::m_instance = 0;

Exaro * Exaro::instance()
{
    if (!m_instance)
	m_instance = new Exaro();

    return Exaro::m_instance;
}

void Exaro::deleteInstance()
{
    if (m_instance)
    {
	delete m_instance;
	m_instance = 0;
    }
}

Exaro::Exaro(QObject * parent)
	:QObject(parent)
{
    qDebug("Exaro CONSTRUCTOR 1");
     m_reportEngine = new Report::ReportEngine(this);
     initMe();
}

Exaro::Exaro(QString &pluginsPath, QObject * parent)
{
    qDebug("Exaro CONSTRUCTOR 2");
    m_reportEngine = new Report::ReportEngine(pluginsPath, this);
    initMe();
}

void Exaro::initMe()
{
//    m_lastReportId = 0;
    m_report = 0;
    m_iconSize = 0;
    m_askBeforeExit = 0;
    m_buttonStyle = Qt::ToolButtonIconOnly;
}

void Exaro::setDatabase(QSqlDatabase db)
{
    m_database = db;
}

bool Exaro::show()
{
    if (!m_database.isValid())
    {
	m_database = QSqlDatabase::database();
	if (!m_database.isValid())
	{
	    m_lastError = "Exaro has no valid database connection.\nYou mast setup database first";
	    return false;
	}
    }
    if (m_fileName.isEmpty())
    {
	m_lastError = "Exaro has no report file name";
	return false;
    }

    //m_report = m_reportEngine->loadReport(m_fileName);
    m_report->setDatabase(m_database);
    m_report->setParent( this );
//    if (m_iconSize)
	connect(m_report, SIGNAL(beforePreviewShow(Report::PreviewDialog*)), this, SLOT(beforePreviewShow(Report::PreviewDialog*)) );

    m_report->exec();
}

void Exaro::setFile(QString fileName)
{
    m_fileName = fileName;
    m_report = m_reportEngine->loadReport(m_fileName);
}

void Exaro::setIconSize(int size)
{
    m_iconSize = size;
}

void Exaro::setToolButtonStyle (Qt::ToolButtonStyle style)
{
    m_buttonStyle = style;
}

void Exaro::setAskBeforeExit(bool b)
{
    m_askBeforeExit = b;
}

QString Exaro::lastError()
{
    return m_lastError;
}

Report::ReportEngine * Exaro::engine()
{
    return m_reportEngine;
}

void Exaro::beforePreviewShow(Report::PreviewDialog* d)
{
    qDebug("setup Priveiw");
    d->setIconSize(m_iconSize);
    d->setShowExitConfirm( m_askBeforeExit );
    d->setToolButtonStyle(m_buttonStyle);
	d->setShowExitConfirm( false );
}

QStringList Exaro::variables()
{
    return m_report->scriptVars().keys();
}

void Exaro::setVar(QString name, QVariant value)
{
    m_report->setReportGlobalValue(name, value);
}

/*
Report::ReportInterface* lastReport()
{
    return m_report;
}
*/
