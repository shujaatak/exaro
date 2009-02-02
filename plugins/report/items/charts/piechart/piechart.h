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

#ifndef PIECHART_H
#define PIECHART_H

#include "abstractchartitem.h"

class QAbstractItemModel;

class PieChart : public AbstractChartItem
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);
	Q_PROPERTY(QStringList dataList READ dataList WRITE setDataList)
//	Q_PROPERTY(QFile dataFile READ dataFile WRITE setDataFIle)
	Q_PROPERTY(int startAngle READ startAngle() WRITE setStartAngle)

public:
/*
    enum LegendFlag {
        NoFlags=0, Shadow=1, SeriesClickable=2
    };
     Q_DECLARE_FLAGS(LegendFlags, LegendFlag)
    ChartLegend(LegendFlags f=NoFlags){ m_flags = f;}
    LegendFlags flags() const{ return m_flags;}
    void setFlags(LegendFlags f){ m_flags = f; }
*/
	PieChart(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	bool canContain(QObject * object);


	int legendWidth() const;
 	void setLegendWidth(const int& theValue);

	void setStartAngle(int deg );
	int startAngle();

	void openFile(const QString &path = QString());
	void calculateTotalValue();

	QStringList dataList();
	void setDataList(QStringList chartData);

//	QFile dataFile();
//	void setDataFile(QFile file);

/*
	qreal xRadius();
	void setXRadius(qreal radius);

	qreal yRadius();
	void setYRadius(qreal radius);
*/
	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);
private:
	void drawChart(QPainter * painter, const QStyleOptionGraphicsItem * option);
	void drawLegend(QPainter * painter, const QStyleOptionGraphicsItem * option);
	
	QSize chartTitleSize() const;
	QSize xTitleSize() const;
   QSize yTitleSize() const;
	QSize rowNameSize() const;
   QSize valueSize() const;

	QFont m_legendFont;

	int m_legendWidth;
	int m_startAngle;
	double m_totalValue;
   int m_validItems;
	QRect chartRect;

	QAbstractItemModel* model;
	QStringList m_chartData;

	//qreal m_xRadius, m_yRadius;
};

#endif
