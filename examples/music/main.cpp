#include <QApplication>
#include <QDebug>

#include <QSqlDatabase>
#include "reportengine.h"

#define error 1
#define ok 0
int main(int argc, char **argv)
{
	if (argc<3)
	{
		qCritical()<<"Error: insuficient params\n\t./musicExample sqlitedatabase,db report.bdrt\n";
		return error;
	}

        QApplication app(argc, argv);

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(argv[1]); // setup a database

        if (!db.open())
	{
		qCritical()<<"Error: Can't open database";
		return error;
	}

        Report::ReportInterface* m_report=0;
        Report::ReportEngine m_reportEngine;
	m_report = m_reportEngine.loadReport(argv[2]); // open report

        if (!m_report)
	{
		qCritical()<<"Error: Can't open the report";
		return error;
	}

        m_report->setDatabase(db); // sets the report database

        if (!m_report->exec()) // and finaly, exec report
	{
		qCritical()<<"Error: Can't exec the report";
                delete m_report;
                return error;
        }
        delete m_report;
        return ok;
}
