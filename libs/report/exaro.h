#ifndef EXAROAPI_H
#define EXAROAPI_H

#include <QObject>
#include <QSqlDatabase>
#include <reportengine.h>
//#include <reportinterface.h>


class Exaro : public QObject
{
    Q_OBJECT
public:
    Exaro(QObject * parent = 0);
    Exaro(QString &pluginsPath, QObject * parent = 0);

    Report::ReportEngine * engine();
    bool show();
    QString lastError();
    void setFile(QString fileName);
    void setDatabase(QSqlDatabase db);
//    Report::ReportInterface* lastReport();
private:
    void initMe();
private:
    QString m_lastError;
    QSqlDatabase m_database;
    Report::ReportEngine * m_reportEngine;
    Report::ReportInterface* m_report;
//    QHash<int,Report::ReportInterface*> reports;

//    int m_lastReportId;
    QString m_fileName;

};
#endif // EXAROAPI_H
