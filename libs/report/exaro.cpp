#include "exaro.h"


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

    m_report = m_reportEngine->loadReport(m_fileName);
    m_report->setDatabase(m_database);
    m_report->setParent( this );
    m_report->exec();
}

void Exaro::setFile(QString fileName)
{
    m_fileName = fileName;
}

QString Exaro::lastError()
{
    return m_lastError;
}

Report::ReportEngine * Exaro::engine()
{
    return m_reportEngine;
}

/*
Report::ReportInterface* lastReport()
{
    return m_report;
}
*/
