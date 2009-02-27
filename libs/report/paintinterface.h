#ifndef PAINTINTERFACE_H
#define PAINTINTERFACE_H

#include <QObject>
#include <QScriptEngine>
#include <QPainter>
#include "layoutmanager.h"

namespace Report
{

class PageInterface;
class ReportInterface;

class PaintInterface : public QObject
{
public:
    PaintInterface(ReportInterface * report, QObject * parent = 0);

    void prepareCurrentPage();
    void postprocessCurrentPage();
private:
    void run();
    void paintBand(BandInterface * band);
    void initItem();
    void showError(QString err);
    void paintObjects(ItemInterface * item, QPointF translate, const QRectF & clipRect);
private:
    BandList  listTop;
    BandList  listBottom;
    BandList  listFree;

    ReportInterface * m_report;
    int currentPageNumber;
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
