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

#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QLinearGradient>
#include "piechart.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(piechart);
}

PieChart::PieChart(QGraphicsItem* parent, QObject* parentObject) : ChartInterface(parent, parentObject), 
	m_barsIdentation(1/UNIT), 
	m_showLabels(false),
	m_toColorFactor(1),
	m_drawBorder(false)/*,
	m_show3d(false)*/
{
	initMyResource();
}

QRectF PieChart::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void PieChart::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{

	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;
	if (option->type == QStyleOption::SO_GraphicsItem)
		//drawSelection(painter, boundingRect());
		painter->drawEllipse(boundingRect());		
	setupPainter(painter);
	adjustRect(rect);

	

	QList<ChartInterface::_chartValue> val=values();
	if (!val.size())
		return;

	qreal suma=0;
	
	foreach(ChartInterface::_chartValue cv, val)
	{
		
			suma +=cv.value;
	}			
	painter->setBrush(brush());
	painter->fillRect(rect,brush());	
	int unghi_dela=0;
	int x_curent,y_curent;	
	qreal x_centru,y_centru,raza;

	x_curent=rect.x()+rect.width();
	y_curent=rect.y();
	
	x_centru=rect.x()+rect.width()/2;
	y_centru=rect.y()+rect.height()/2;
	int b=rect.height()/2;
	int a=rect.width()/2;
	int circumferinta=0,contor=0;
	foreach(ChartInterface::_chartValue cv, val)
	{	
		contor++;
		QLinearGradient lg(QPointF(15,0), QPointF(15,0));
		lg.setSpread(QGradient::ReflectSpread);
		lg.setColorAt(0, cv.color);		
		lg.setColorAt(1, QColor(cv.color.red()*m_toColorFactor, cv.color.green()*m_toColorFactor, cv.color.blue()*m_toColorFactor, cv.color.alpha()));			
		painter->setBrush(lg);

		int pondere;
		pondere=static_cast<int>((100*cv.value)/suma+0.5);
		int unghi_deschidere;
		unghi_deschidere=static_cast<int>((pondere*360)/100 +0.5);										
		if(m_showLabels)
		{
			x_curent=a*cos(((2*3.14)/360)*(circumferinta+unghi_deschidere/2)) + x_centru;
			y_curent=-b*sin(((2*3.14)/360)*(circumferinta+unghi_deschidere/2)) + y_centru;
			if(x_curent >=x_centru)
			{									
				painter->setPen(cv.color);
				painter->drawChord(QRectF(x_curent,y_curent,10,10),0,360*16);
				painter->drawLine(x_curent,y_curent,x_curent+100,y_curent);
				painter->setPen(Qt::black);
				painter->drawText (QPointF( x_curent+100,y_curent),QString("%1").arg(cv.value));			
			}
			else
			{
				painter->setPen(cv.color);
				painter->drawChord(QRectF(x_curent,y_curent,10,10),0,360*16);
				painter->drawLine(x_curent,y_curent,x_curent-100,y_curent);
				painter->setPen(Qt::black);	
				painter->drawText (QPointF( x_curent-100,y_curent),QString("%1").arg(cv.value),0,1);					
				//painter->drawText (QRectF( x_curent-100,y_curent,100,0),Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(cv.value));
			}
		}
		if(val.size() == contor)		
			unghi_deschidere = 360-circumferinta;					
		else		
			circumferinta +=unghi_deschidere;
		QRectF rectPie(rect.x(),rect.y(),rect.width(),rect.height());		
		painter->drawPie(rectPie,unghi_dela*16,(unghi_deschidere)*16);						
		unghi_dela +=unghi_deschidere;
		
	}			
}

int PieChart::distanta(QPointF from,QPointF to)
{
	return  static_cast<int>(sqrt( (from.x()-to.x())* (from.x()-to.x()) + (from.y()-to.y())*(from.y()-to.y()) ));
}

QIcon PieChart::toolBoxIcon()
{
	return QIcon(":/piechart.png");
}

QString PieChart::toolBoxText()
{
	return tr("Pie chart");
}

QString PieChart::toolBoxGroup()
{
	return tr("Charts");
}

QObject * PieChart::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new PieChart(parent, parentObject);
}

int PieChart::barsIdentation()
{
	return m_barsIdentation;
}

void PieChart::setBarsIdentation(int barsIdentation)
{
	if (barsIdentation<1)
		barsIdentation=1;
	m_barsIdentation=barsIdentation;
	update();
}

/*
bool PieChart::show3d()
{
	return m_show3d;
}
void PieChart::setShow3d(bool show3d)
{
	m_show3d=show3d;
	update();
}
*/

bool PieChart::showLabels()
{
	return m_showLabels;
}

void PieChart::setShowLabels(bool showLabels)
{
	m_showLabels=showLabels;
	update();
}

bool PieChart::showGrid()
{
	return m_showGrid;
}
void PieChart::setShowGrid(bool showGrid)
{
	m_showGrid=showGrid;
	update();
}

qreal PieChart::toColorFactor()
{
	return m_toColorFactor;
}

void PieChart::setToColorFactor(qreal toColorFactor)
{
	if (toColorFactor>10)
		toColorFactor=10;

	if (toColorFactor<0.1)
		toColorFactor=0.1;
	m_toColorFactor=toColorFactor;
	update();
}

bool PieChart::drawBorder()
{
	return m_drawBorder;
}
void PieChart::setDrawBorder(bool drawBorder)
{
	m_drawBorder=drawBorder;
	update();
}

Q_EXPORT_PLUGIN2(pieChart, PieChart)
