/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2009 Florina Nacu                                       *
 *   nacuflorina@gmail.com                                                 *
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

#ifndef BARCHART_H
#define BARCHART_H

#include "chartinterface.h"

class PieChart : public Report::ChartInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ChartInterface);

//	Q_PROPERTY(bool show3d READ show3d WRITE setShow3d)
	Q_PROPERTY(bool showLabels READ showLabels WRITE setShowLabels)
	Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid)
	Q_PROPERTY(bool drawBorder READ drawBorder WRITE setDrawBorder)
	Q_PROPERTY(qreal toColorFactor READ toColorFactor WRITE setToColorFactor)

public:
	PieChart(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

//	bool show3d();
//	void setShow3d(bool show3d);

	bool drawBorder();
	void setDrawBorder(bool drawBorder);

	bool showGrid();
	void setShowGrid(bool showGrid);

	bool showLabels();
	void setShowLabels(bool showLabels);

	qreal toColorFactor();
	void setToColorFactor(qreal toColorFactor);

        bool findNegativeValue(QList<ChartInterface::_chartValue> & );


private:	
//	bool m_show3d;
	bool m_showLabels;
	qreal m_toColorFactor;
	bool m_drawBorder;
	bool m_showGrid;
};

#endif
