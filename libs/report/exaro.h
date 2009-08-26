#ifndef EXAROAPI_H
#define EXAROAPI_H

#include <QObject>
#include <QSqlDatabase>

namespace Report
{
    class ReportEngine;
    class ReportInterface;
    class PreviewDialog;
}

class Exaro : public QObject
{
    Q_OBJECT
public:
    Exaro(QObject * parent = 0);
    Exaro(QString &pluginsPath, QObject * parent = 0);

    static Exaro* instance();
    static void deleteInstance();

    bool show();

    Report::ReportEngine * engine();
    QString lastError();
    void setFile(QString fileName);
    void setDatabase(QSqlDatabase db);
    void setIconSize(int size);
    void setAskBeforeExit(bool b);
    void setToolButtonStyle (Qt::ToolButtonStyle style);
//    Report::ReportInterface* lastReport();

private:
    void initMe();

private slots:
    void beforePreviewShow(Report::PreviewDialog * d);

private:
    static Exaro * m_instance;
    QString m_lastError;
    QSqlDatabase m_database;
    Report::ReportEngine * m_reportEngine;
    Report::ReportInterface* m_report;
//    QHash<int,Report::ReportInterface*> reports;

//    int m_lastReportId;
    QString m_fileName;
    int m_iconSize;
    bool m_askBeforeExit;
    Qt::ToolButtonStyle m_buttonStyle;
};
#endif // EXAROAPI_H
