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

bool PieChart::findNegativeValue(QList<ChartInterface::_chartValue> &valori)
{
    // need optimize
    foreach(ChartInterface::_chartValue cv, valori)
    {
            if(cv.value < 0)
                    return true;
    }
    return false;
}

void PieChart::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{

	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;
	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());
	setupPainter(painter);
	adjustRect(rect);

	QList<ChartInterface::_chartValue> val=values();
        QVector<qreal> val_prelucrate ;
	if (!val.size())
		return;

	qreal suma=0;
        qreal suma_prelucrata=0;
	
	foreach(ChartInterface::_chartValue cv, val)
	{
		suma +=cv.value;
	}

        if(findNegativeValue(val))
        {               
                int pos=0;                
                foreach(ChartInterface::_chartValue cv, val)
                {
                        qreal cv_prelucrata;
                        if(val.value(0).value < val.value(val.size()-1).value)
                        {
                            // sorted ascendent                           
                            cv_prelucrata = cv.value + abs(val.value(0).value)+4;
                        }
                        else                            
                        {                                                        
                            cv_prelucrata = cv.value + abs(val.value(val.size()-1).value)+3;
                        }
                            // sorted descendent
                        val_prelucrate.append(cv_prelucrata);
                        suma_prelucrata += val_prelucrate.value(pos);
                        pos++;
                }                
        }
	painter->setBrush(brush());
	painter->fillRect(rect,brush());	
	int unghi_dela=0;
	int x_curent,y_curent;
	qreal x_centru,y_centru;

	x_curent=rect.x()+rect.width();
	y_curent=rect.y();
	
	x_centru=rect.x()+rect.width()/2;
	y_centru=rect.y()+rect.height()/2;
	int b=rect.height()/2;
	int a=rect.width()/2;
	int circumferinta=0,contor=0;
	int x_line=0,y_line=0;
	foreach(ChartInterface::_chartValue cv, val)
	{	
		contor++;
		QLinearGradient lg(QPointF(15,0), QPointF(15,0));
		lg.setSpread(QGradient::ReflectSpread);
		lg.setColorAt(0, cv.color);		
		lg.setColorAt(1, QColor(cv.color.red()*m_toColorFactor, cv.color.green()*m_toColorFactor, cv.color.blue()*m_toColorFactor, cv.color.alpha()));			
		painter->setBrush(lg);                
		int pondere;
                if(findNegativeValue(val))
                {
                    pondere=static_cast<int>((100 * val_prelucrate.value(contor-1))/suma_prelucrata+0.5);
                }
                else
                    pondere=static_cast<int>((100*cv.value)/suma+0.5);                
		int unghi_deschidere;
		unghi_deschidere=static_cast<int>((pondere*360)/100 +0.5);
		if(m_showLabels)
		{
			x_curent=a*cos(((2*3.14)/360)*(circumferinta+unghi_deschidere/2)) + x_centru;
			y_curent=-b*sin(((2*3.14)/360)*(circumferinta+unghi_deschidere/2)) + y_centru;
			if(x_curent >=x_centru)
			{
				if((x_line>x_centru) && ((y_curent+50) >(y_line-50)) && (y_line>0))
					y_line=y_curent-50;
				else
					y_line=y_curent;
				x_line=x_curent+( (rect.x()+rect.width())-x_curent )+100;
				painter->setPen(cv.color);
				painter->drawChord(QRectF(x_curent,y_curent,10,10),0,360*16);
				painter->drawLine(x_curent,y_curent,x_line,y_line);
				x_curent=x_line;
				y_curent=y_line;
				y_line=y_line+50;
				painter->drawLine(x_curent,y_curent,x_line,y_line);
				painter->drawChord(QRectF(x_line,y_line,10,10),0,360*16);
				painter->setPen(Qt::black);
				painter->drawText (QPointF( x_line,y_line),QString("%1").arg(cv.value));
			}
			else
			{
				if((x_line<x_centru) && ((y_curent+50) <(y_line+50)) && (y_line>0))
					y_line=y_curent+50;
				else
					y_line=y_curent;
				x_line=x_curent-(x_curent-rect.x() )-100;
				painter->setPen(cv.color);
				painter->drawChord(QRectF(x_curent,y_curent,10,10),0,360*16);
				painter->drawLine(x_curent,y_curent,x_line,y_line);
				x_curent=x_line;
				y_curent=y_line;
				y_line=y_line+50;
				painter->drawLine(x_curent,y_curent,x_line,y_line);
				painter->drawChord(QRectF(x_line,y_line,10,10),0,360*16);
				painter->setPen(Qt::black);
				painter->drawText (QPointF( x_line-painter->fontMetrics().width(QString("%1").arg(cv.value)),y_line),QString("%1").arg(cv.value));
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
