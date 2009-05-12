#ifndef PAINTINTERFACE_H
#define PAINTINTERFACE_H

#include <QObject>
#include <QScriptEngine>
#include <QPainter>
#include "dataset.h"
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
    //enum PrintMode {pmNormal, pmNewPage, pmClosePage, pmReportEnd, pmCheckIteration};
public:
    PaintInterface(ReportInterface * report, QObject * parent = 0);
    ~PaintInterface();

    void prepareCurrentPage();
    void postprocessCurrentPage();
    int currentPageNumber();
    int currentDatasetRow();
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
    void processBand(BandInterface * band);
    void initBands();
    void paintObjects(ItemInterface * item, QPointF translate, const QRectF & clipRect);
    void processDataset(QString datasetName, BandInterface * band = 0);
    bool canPaint(BandInterface * band);
    void processPage();
//    void exportRecord(const QSqlRecord & record, QDomElement & el);

private:
    BandList  listTop;		    //top positioned bands
    BandList  listBottom;	    //bottom positioned bands
    BandList  listFree;		    //free positioned band
    BandList  currentGroup;	    //current processing bands group - for example dataset grouped bands
    BandList  bandDone;		    //list of bands what alrady processed

    ReportInterface * m_report;
    int m_currentPageNumber;	    //absolute page number

    DataSet * m_currentDataset;
    int m_currentDatasetRow;	    // can't be changed - use for positioning in dataset
    int m_currentLineNumber;	    //can be changed by Bands or in user script
    BandInterface * currentBand;    //current processed band
    PageInterface * m_currentPage;  //current processed page

    PaintDevice * m_printer;

    QPainter m_painter;

    //QSize m_currentSize;
    /*
    PageInterface * m_page;
    QScriptEngine * m_scriptEngine;
    QPainter * m_painter;
*/
    QRectF freeSpace;		    //current page free (unpainted) space - space usefull for bands

friend class ReportInterface;
};
}
#endif // PAINTINTERFACE_H
