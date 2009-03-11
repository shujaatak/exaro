#ifndef PAINTINTERFACE_H
#define PAINTINTERFACE_H

#include <QObject>
#include <QScriptEngine>
#include <QPainter>
#include "sqlquery.h"
#include "layoutmanager.h"

#include "pageinterface.h"
#include "paintdevice.h"


namespace Report
{

class PageInterface;
class ReportInterface;

class PaintInterface : public QThread
{
    Q_OBJECT
public:
    enum PrintMode {pmNormal, pmNewPage, pmClosePage, pmReportEnd, pmCheckIteration};
public:
    PaintInterface(ReportInterface * report, QObject * parent = 0);

    void prepareCurrentPage();
    void postprocessCurrentPage();
    int currentPageNumber();
    int currentQueryRow();
    void setDetailNumber(int num);
    //QString currentBandNameInThead();
    //void checkAllIteration(QString queryName = QString());  //add band to array and process it in all iteration in query queryName
							    // or all queryes if queryName.isNull
							    // pmCheckIteration use by engine to process Band
    void newPage();

public slots:
    void showError(QString err);
signals:
    void showProcess(QString str);

private:
    void run();
    void finish(QString error = "");
    void processBand(BandInterface * band, PrintMode pMode = pmNormal);
    void initBands();
    void paintObjects(ItemInterface * item, QPointF translate, const QRectF & clipRect);
    void processQuery(QString queryName, BandInterface * band = 0);
    bool canPaint(BandInterface * band);
    void processPage();
//    void exportRecord(const QSqlRecord & record, QDomElement & el);

private:
    BandList  listTop;
    BandList  listBottom;
    BandList  listFree;
    BandList  currentGroup;
    BandList  bandDone;

    ReportInterface * m_report;
    int m_currentPageNumber;

    SqlQuery * m_currentQuery;
    int m_currentQueryRow;	    // can't be changed - use for positioning in query
    int m_currentLineNumber;	    //can be changed by Bands or in user script
    BandInterface * currentBand;
    PageInterface * m_currentPage;

    PaintDevice * m_printer;

    QPainter m_painter;

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
