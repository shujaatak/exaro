/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bogdan@licentia.eu                                                    *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef BARCHART_H
#define BARCHART_H

#include "chartinterface.h"

class BarChart : public Report::ChartInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ChartInterface);

	Q_PROPERTY(bool showLabels READ showLabels WRITE setShowLabels)
	Q_PROPERTY(bool drawBorder READ drawBorder WRITE setDrawBorder)
	Q_PROPERTY(int barsIdentation READ barsIdentation WRITE setBarsIdentation)
	Q_PROPERTY(qreal toColorFactor READ toColorFactor WRITE setToColorFactor)

public:
	BarChart(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	int barsIdentation();
	void setBarsIdentation(int barsIdentation);

	bool drawBorder();
	void setDrawBorder(bool drawBorder);

	bool showLabels();
	void setShowLabels(bool showLabels);

	qreal toColorFactor();
	void setToColorFactor(qreal toColorFactor);

private:
	int m_barsIdentation;
	bool m_showLabels;
	qreal m_toColorFactor;
	bool m_drawBorder;
};

#endif
