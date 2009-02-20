/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
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

#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QFont>

#include "piechart.h"
#include "bandinterface.h"
#include "QStandardItemModel"

#ifndef M_PI
#define M_PI 3.1415927
#endif

#define MIN_WIDTH = 150
#define MIN_HEIGTH = 150
//#define DEBUG

inline void initMyResource()
{
	Q_INIT_RESOURCE(piechart);
}

PieChart::PieChart(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject)
	,m_legendWidth(200), m_startAngle(120), m_legendFont("Helvetica [Cronyx]" , 10)
{
	initMyResource();

	model = new QStandardItemModel(8, 2, this);
   //model->setHeaderData(0, Qt::Horizontal, tr("Label"));
   //model->setHeaderData(1, Qt::Horizontal, tr("Quantity"));

	openFile(":qtdata.cht");
	setWidth(1000);
	setHeight(500);
	setMinHeight(300);
	setMinWidth(700);
}

bool PieChart::canContain(QObject * object)
{
	return (!dynamic_cast<Report::BandInterface*>(object) && dynamic_cast<Report::ItemInterface*>(object));
}

QRectF PieChart::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

QSize PieChart::chartTitleSize() const
{
/*
//qDebug(qPrintable(QString::number(__LINE__)));
    QSize s;
    if(!(_flags & ChartTitle) || chartTitle().isNull())
        return QSize(chartSize().width()+yTitleSize().height()+4+_legendwidth+4+valueSize().width(), 0);
    QVariant v = chartTitle(Qt::SizeHintRole);
    if(v.isValid()) {
        s= qvariant_cast<QSize>(v);
        s.setWidth(chartSize().width());
        return s;
    }
    s.setWidth(chartSize().width()+yTitleSize().height()+4+_legendwidth+4+valueSize().width());

    v = chartTitle(Qt::FontRole);
    if(v.isValid()) {
        QFont f = qvariant_cast<QFont>(v);
        s.setHeight(QFontMetrics(f).height());
    } else {
        s.setHeight(_canvasVMargin);
    }
    return s;
 */
	return QSize(50,50);
}

QSize PieChart::xTitleSize() const
{
/*
//qDebug("QSize ColumnChartView::xTitleSize( ) const");
    QSize s;

    if(!(_flags & XTitle) || xTitle().isNull())
        return QSize(chartSize().width(), 0);

    QVariant v = xTitle(Qt::SizeHintRole);
    if(v.isValid()) {
        s= qvariant_cast<QSize>(v);
        s.setWidth(chartSize().width());
        return s;
    }
    s.setWidth(chartSize().width());
    v = xTitle(Qt::FontRole);
    if(v.isValid()) {
        QFont f = qvariant_cast<QFont>(v);
        s.setHeight(QFontMetrics(f).height());
    } else {
        s.setHeight(_canvasVMargin);
    }
    return s;
*/
	return QSize(50,50);
}


QSize PieChart::yTitleSize() const
{
/*
//    qDebug("ColumnChartView::yTitleSize( ) const");
    QSize s;
    if(!(_flags & YTitle) || yTitle().isNull())
        return QSize(chartSize().width(), 0);
    QVariant v = yTitle(Qt::SizeHintRole);
    if(v.isValid()) {
        s= qvariant_cast<QSize>(v);
        s.setWidth(chartSize().width());
        return s;
    }
    s.setWidth(chartSize().width());
    v = yTitle(Qt::FontRole);
    if(v.isValid()) {
        QFont f = qvariant_cast<QFont>(v);
        s.setHeight(QFontMetrics(f).height());
    } else {
        s.setHeight(_canvasVMargin);
    }
    return s;
*/
	return QSize(50,50);
}

QSize PieChart::rowNameSize() const
{
//    qDebug("QSize ColumnChartView::rowNameSize( ) const");
//    return QSize(0, QFontMetrics(font()).height());
	return QSize(0,0);
}

QSize PieChart::valueSize() const
{
/*
    int m = QFontMetrics(font()).width(QString::number(maximumValue()));
    return QSize(m+4, 0);
*/
	return QSize(0,0);
}

int PieChart::legendWidth() const
{
      return m_legendWidth;
}

void PieChart::setLegendWidth(const int& theValue)
{
     m_legendWidth = theValue;
}

void PieChart::setStartAngle(int deg )
{
     m_startAngle = deg;
}

int PieChart::startAngle()
{
     return m_startAngle;
}

QStringList PieChart::dataList()
{
     return m_chartData;  
}

void PieChart::setDataList(QStringList chartData)
{
       m_chartData = chartData;
}

void PieChart::openFile(const QString &path)
{
	QString fileName;

	fileName = path;

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            QString line;

            model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

            int row = 0;
            do {
                line = stream.readLine();
                if (!line.isEmpty()) {

                    model->insertRows(row, 1, QModelIndex());

                    QStringList pieces = line.split(",", QString::SkipEmptyParts);
                    model->setData(model->index(row, 1, QModelIndex()),
                                   pieces.value(0));
                    model->setData(model->index(row, 0, QModelIndex()),
                                   pieces.value(1));
                    model->setData(model->index(row, 0, QModelIndex()),
                                   QColor(pieces.value(2)), Qt::DecorationRole);
                    row++;
                }
            } while (!line.isEmpty());

            file.close();
            qDebug("Loaded %s",qPrintable(fileName));
				calculateTotalValue();
        }
    }
}


void PieChart::calculateTotalValue()
{
	qDebug("PieChart::calculateTotalValue()");
    m_validItems = 0;
    m_totalValue = 0.0;

    for (int row = 0; row < model->rowCount(); ++row) {

        QModelIndex index = model->index(row, 1);
        double value = model->data(index).toDouble();

        if (value > 0.0) {
            m_totalValue += value;
            m_validItems++;
        }
    }
}

void PieChart::drawChart(QPainter * painter, const QStyleOptionGraphicsItem * option)
{

	//painter->fillRect(chartRect , QColor(220,220,220));
   painter->drawEllipse(chartRect);

	double startAngle = m_startAngle;
	int row;

   for (row = 0; row < model->rowCount(); ++row) {

       QModelIndex index = model->index(row, 1);
       double value = model->data(index).toDouble();

       if (value > 0.0) {
           double angle = 360*value/m_totalValue;

           QModelIndex colorIndex = model->index(row, 0);
           QColor color = QColor(model->data(colorIndex,
                           Qt::DecorationRole).toString());

           painter->setBrush(QBrush(color));

           painter->drawPie(chartRect, int(startAngle*16), int(angle*16));

           startAngle += angle;
       }
   }
}

void PieChart::drawLegend( QPainter * painter, const QStyleOptionGraphicsItem * option)
{
//qDebug(qPrintable(QString::number(__LINE__)));
    painter->save();
    QFontMetrics fm(m_legendFont);
    painter->translate(4 + chartRect.x() + chartRect.width(), chartRect.y() );
#ifdef DEBUG

    painter->save();
    QPen pn(Qt::black);
    pn.setStyle(Qt::DashLine);
    painter->setPen(pn);
    painter->drawRect(QRect(0,0, m_legendWidth+3, chartRect.height()));
    painter->restore();
#endif

    painter->drawRect(0, 0, m_legendWidth, 6+(model->rowCount())*(m_legendFont.pointSize()+4));
    if(/*_legend.flags() & ChartLegend::Shadow*/ true) {
        painter->save();
        QPen pn = painter->pen();
        pn.setWidth(3);
        pn.setColor(QPalette::Shadow);
        painter->setPen(pn);
        int dl = (model->rowCount())*(m_legendFont.pointSize()+4);
        painter->drawLine(5,8+dl, m_legendWidth, 8+dl);
        painter->drawLine(2+m_legendWidth, 5, 2+m_legendWidth, 8+dl);
        painter->restore();
    }

	 painter->setFont(m_legendFont);
    for(int i=0;i<model->rowCount();i++) {
        painter->setBrush(qvariant_cast<QColor>( model->data(model->index(i, 0),  Qt::DecorationRole) ) );
        painter->drawRect(5, 6+i*(m_legendFont.pointSize()+4), m_legendFont.pointSize(), m_legendFont.pointSize());
        painter->drawText(5+m_legendFont.pointSize()+4, m_legendFont.pointSize()+6+i*(m_legendFont.pointSize()+4),  model->data(model->index(i, 0) ).toString());
			//qDebug("print String %s", qPrintable());
    }

    painter->restore();
}


void PieChart::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, rect);

	setupPainter(painter);

	adjustRect(rect);

	int x0_offset = 4 + yTitleSize().height() + 4 + valueSize().width() + 4;
	int y0_offset = 4 + chartTitleSize().height() + 4;
	int x1_offset = 4 + m_legendWidth + 4;
	int y1_offset = 4;

   chartRect = QRect(x0_offset, y0_offset,
				rect.width() - x0_offset - x1_offset , rect.height() - y0_offset - y1_offset);

	painter->drawRect(rect);

   painter->translate(rect.x(), rect.y() );

   drawChart(painter, option);
	//if(_flags & Legend)
        drawLegend(painter, option);


/*
	if(_flags & VerticalGrid)
        drawGridLines(&painter);
	drawAxis(&painter);
	if(_flags & Legend)
        drawLegend(&painter);
	if(_flags & VerticalScale)
        drawVerticalScale(&painter);
	if(_flags & ChartTitle)
        drawChartTitle(&painter);
	if(_flags & XTitle)
        drawXTitle(&painter);
	if(_flags & YTitle)
        drawYTitle(&painter);
	drawRowNames(&painter);
*/
//	painter->drawRoundRect(rect/*, xRadius(), yRadius()*/);

}

QIcon PieChart::toolBoxIcon()
{
	return QIcon(":/piechart.png");
};

QString PieChart::toolBoxText()
{
	return tr("PieChart");
}

QString PieChart::toolBoxGroup()
{
	return tr("Charts");
}

QObject * PieChart::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new PieChart(parent, parentObject);
}

Q_EXPORT_PLUGIN2(piechart, PieChart)

