#ifndef PAINTINTERFACE_H
#define PAINTINTERFACE_H

#include <QObject>
#include <QScriptEngine>
#include <QPainter>
#include "sqlquery.h"
#include "layoutmanager.h"

namespace Report
{

class PageInterface;
class ReportInterface;

class PaintInterface : public QObject
{
    Q_OBJECT
public:
    PaintInterface(ReportInterface * report, QObject * parent = 0);

    void prepareCurrentPage();
    void postprocessCurrentPage();

public slots:
    void showError(QString err);

private:
    void run();
    void finish(QString error = "");
    void paintBand(BandInterface * band);
    void initItem();
    void paintObjects(ItemInterface * item, QPointF translate, const QRectF & clipRect);
    void processQuery(QString queryName, BandInterface * band = 0);
    bool canPaint(BandInterface * band);
    void newPage();

private:
    BandList  listTop;
    BandList  listBottom;
    BandList  listFree;
    BandList  currentGroup;
    BandList  bandDone;

    ReportInterface * m_report;
    int currentPageNumber;

    SqlQuery * m_currentQuery;
    int m_currentQueryRow;
    //QSize m_currentSize;
    /*
    PageInterface * m_page;
    QScriptEngine * m_scriptEngine;
    QPainter * m_painter;
*/
    QRectF freeSpace;

friend class ReportInterface;
};
}
#endif // PAINTINTERFACE_H
