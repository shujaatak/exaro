/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2009 by Alexander Mikhalov  (aka alFoX)                 *
 *              alexmi3@rambler.ru                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

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
    Report::DataSet * currentDataset();
    int currentDetailNumber();
    void setDetailNumber(int num);
    void newPage();
    void processBand(BandInterface * band);
    void prepareDatasets();
    void processDataset(DataSet * dtst);
    QRectF pageFreeSpace();
    void setPageFreeSpace (QRectF rect);
    BandInterface * lastProcessedBand();

public slots:
    void showError(QString err);
signals:
    void showProcess(QString str);
    void processBandBefore(BandInterface *band);
    void processBandAfter(BandInterface *band);
    void processDatasetBefore(DataSet * dtst);
    void processDatasetAfter(DataSet * dtst);
    void newPageBefore();
    void newPageAfter();
    void closePageBefore();
    void closePageAfter();

private:
    void run();
    void finish(QString error = "");
    void initBands();
//    void processDataset(QString datasetName, BandInterface * band = 0);
    bool canPaint(BandInterface * band);
    void processTemplatePage();
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
    BandInterface * m_lastProcessedBand;

    PaintDevice * m_printer;

    QPainter m_painter;

    QRectF freeSpace;		    //current page free (unpainted) space - space usefull for bands

friend class ReportInterface;
};
}
#endif // PAINTINTERFACE_H
