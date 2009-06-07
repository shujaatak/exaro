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
    PaintInterface(ReportInterface * report, QObject * parent = 0);
    ~PaintInterface();

    void prepareCurrentPage();
    void postprocessCurrentPage();
    int currentPageNumber();
    int currentDatasetRow();
    QString currentDatasetName();
    void setDetailNumber(int num);
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
//    void processDataset(QString datasetName, BandInterface * band = 0);
    void prepareDatasets();
    void processDataset(DataSet * dtst);
    bool canPaint(BandInterface * band);
    void processPage();
//    void exportRecord(const QSqlRecord & record, QDomElement & el);

private:
    BandList  listTop;		    //top positioned bands
    BandList  listBottom;	    //bottom positioned bands
    BandList  listFree;		    //free positioned band
//    BandList  currentGroup;	    //current processing bands group - for example dataset grouped bands
    BandList  bandDone;		    //list of bands what already processed

    ReportInterface * m_report;
    int m_currentPageNumber;	    //absolute page number

    DataSet * m_currentDataset;
    int m_currentDatasetRow;	    //can't be changed - use for positioning in dataset
    int m_currentLineNumber;	    //can be changed by Bands or in user script
    BandInterface * currentBand;    //current processed band
    PageInterface * m_currentPage;  //current processed page

    PaintDevice * m_printer;

    QPainter m_painter;

    QRectF freeSpace;		    //current page free (unpainted) space - space usefull for bands

friend class ReportInterface;
};
}
#endif // PAINTINTERFACE_H
