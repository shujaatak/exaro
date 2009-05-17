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
#include <QLinearGradient>
//#include <QGLPixelBuffer>

#include "barchart.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(barchart);
}

BarChart::BarChart(QGraphicsItem* parent, QObject* parentObject) : ChartInterface(parent, parentObject), 
	m_barsIdentation(1/UNIT), 
	m_showLabels(false),
	m_toColorFactor(1),
	m_drawBorder(false)/*,
	m_show3d(false)*/
{
	initMyResource();
}

QRectF BarChart::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void BarChart::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{

	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;
	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);
	adjustRect(rect);

	QList<ChartInterface::_chartValue> val=values();
	if (!val.size())
		return;

	qreal maxpv=0;
	qreal minnv=0;
	foreach(ChartInterface::_chartValue cv, val)
	{
		if (cv.value>0 && cv.value>maxpv)
			maxpv=cv.value;

		if (cv.value<0 && cv.value<minnv)
			minnv=cv.value;
	}

	qreal absMaxVal=maxpv-minnv;
	qreal powVal=(absMaxVal<1)?pow(10.0,QString::number(absMaxVal).right(QString::number(absMaxVal).indexOf('.')).length()+1):1;
	maxpv*=powVal;
	minnv*=powVal;

	maxpv=(quint64)maxpv;
	minnv=(quint64)(-minnv);
	minnv=-minnv;

	painter->fillRect(rect,brush());

	if (m_drawBorder)
		painter->drawRect(rect);

	int pw=abs(pen().widthF())?abs(pen().widthF()):1;
	QRectF rc=rect.adjusted(pw/2,pw/2,-pw,-pw);

	qreal f=2;

	qreal chartStep=pow(10.0,(QString::number(absMaxVal).left(QString::number(absMaxVal).indexOf('.')).length())-1)/f;
	qreal powStep=(chartStep<1)?10:1;
	chartStep*=powStep;
	maxpv*=powStep;
	minnv*=powStep;
	powVal*=powStep;

	maxpv=maxpv+((((quint64)maxpv%(quint64)chartStep)?((quint64)chartStep-((quint64)maxpv%(quint64)chartStep)):0))/powVal;
	minnv=minnv-(((-(quint64)minnv%(quint64)chartStep)?((quint64)chartStep-(-(quint64)minnv%(quint64)chartStep)):0))/powVal;
	quint64 maxVal=maxpv-minnv;

	qreal maxHeight=rc.height()-painter->fontMetrics().height();
	qreal valstep=maxHeight/(maxVal/chartStep);

	if (valstep<painter->fontMetrics().height())
	{
		chartStep*=(((quint64)(painter->fontMetrics().height()/valstep))+1);
		valstep=(((quint64)(painter->fontMetrics().height()/valstep))+1)*valstep;
	}

	if (m_showLabels)
	{
#ifdef __GNUC__
#warning FIXME ensure max positive and max negative is visible
#else
#pragma WARNING( FIXME ensure max positive and max negative is visible )
#endif
		qreal maxLabelWidth=0;
		for(int i=0;i<maxVal/chartStep+1+((quint64)maxVal%(quint64)chartStep?1:0);i++)
			if (maxLabelWidth<painter->fontMetrics().width(QString::number((maxVal*i-chartStep*i)/powVal)))
				maxLabelWidth=painter->fontMetrics().width(QString::number((maxVal*i-chartStep*i)/powVal));
		int y=0;
		for(int i=0;i<maxVal/chartStep+1+((quint64)maxVal%(quint64)chartStep?1:0);i++)
		{
			painter->drawText(QRectF(rc.x(),rc.y()+y,maxLabelWidth,painter->fontMetrics().height()),Qt::AlignRight|Qt::AlignVCenter,QString::number((maxpv-chartStep*i)/powVal));
			y+=valstep;
		}

		painter->drawLine(rc.x()+maxLabelWidth+1/UNIT/4,rc.y(),rc.x()+maxLabelWidth+1/UNIT/4,rc.y()+rect.height());
		rc=rc.adjusted(maxLabelWidth+1/UNIT/4,0,0,0);
	}

	if (m_showGrid)
	{
		int y=(double)painter->fontMetrics().height()/2;
		for(int i=0;i<maxVal/chartStep+1+((quint64)maxVal%(quint64)chartStep?1:0);i++)
		{
			painter->drawLine(rc.x(),rc.y()+y,rc.x()+rc.width(),rc.y()+y);
			y+=valstep;
		}
	}

	rc=rc.adjusted(0,(double)painter->fontMetrics().height()/2,0,0);
	int x=m_barsIdentation;
	qreal barWidth=(rc.width()-m_barsIdentation*(val.size()+1))/val.size();
	qreal py=maxHeight/maxVal;
//	if(!m_show3d)
	{
		foreach(ChartInterface::_chartValue cv, val)
		{
			QLinearGradient lg(QPointF(x+barWidth/2,0), QPointF(x+barWidth,0));
			lg.setSpread(QGradient::ReflectSpread);
			lg.setColorAt(0, cv.color);
			lg.setColorAt(1, QColor(cv.color.red()*m_toColorFactor, cv.color.green()*m_toColorFactor, cv.color.blue()*m_toColorFactor, cv.color.alpha()));
			painter->fillRect(QRectF(rc.x()+x,rc.y()+py*maxpv-py*cv.value*powVal,barWidth, py*cv.value*powVal),lg);
			if (m_showLabels)
				painter->drawText(QRectF(rc.x()+x-m_barsIdentation/2, rc.y()+py*maxpv-((cv.value>=0)?painter->fontMetrics().height():0), barWidth+m_barsIdentation, painter->fontMetrics().height()),Qt::AlignCenter,QString("%1").arg(cv.value));
			x+=barWidth+m_barsIdentation;
		}
	}
/*
	else
	{
		QGLPixelBuffer pb(rc.size().toSize());
		if (!pb.makeCurrent())
			qDebug()<<"can't quary context";
		glFlush();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f,-7.0f);       // Translate Into The Screen 7.0 Units
		glRotatef(5,0.0f,1.0f,0.0f);    // Rotate The cube around the Y axis
		glRotatef(5,1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);            // Draw The Cube Using quads
		glColor3f(0.0f,1.0f,0.0f);  // Color Blue
		glVertex3f( 1.0f, 1.0f,-1.0f);      // Top Right Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f,-1.0f);      // Top Left Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f, 1.0f);      // Bottom Left Of The Quad (Top)
		glVertex3f( 1.0f, 1.0f, 1.0f);      // Bottom Right Of The Quad (Top)
		glColor3f(1.0f,0.5f,0.0f);  // Color Orange
		glVertex3f( 1.0f,-1.0f, 1.0f);      // Top Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);      // Top Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);      // Bottom Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f,-1.0f);      // Bottom Right Of The Quad (Bottom)
		glColor3f(1.0f,0.0f,0.0f);  // Color Red    
		glVertex3f( 1.0f, 1.0f, 1.0f);      // Top Right Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);      // Top Left Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);      // Bottom Left Of The Quad (Front)
		glVertex3f( 1.0f,-1.0f, 1.0f);      // Bottom Right Of The Quad (Front)
		glColor3f(1.0f,1.0f,0.0f);  // Color Yellow
		glVertex3f( 1.0f,-1.0f,-1.0f);      // Top Right Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);      // Top Left Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);      // Bottom Left Of The Quad (Back)
		glVertex3f( 1.0f, 1.0f,-1.0f);      // Bottom Right Of The Quad (Back)
		glColor3f(0.0f,0.0f,1.0f);  // Color Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);      // Top Right Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);      // Top Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);      // Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f, 1.0f);      // Bottom Right Of The Quad (Left)
		glColor3f(1.0f,0.0f,1.0f);  // Color Violet
		glVertex3f( 1.0f, 1.0f,-1.0f);      // Top Right Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f, 1.0f);      // Top Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f, 1.0f);      // Bottom Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f,-1.0f);      // Bottom Right Of The Quad (Right)
		glEnd();
		pb.doneCurrent();
		QImage img=pb.toImage();
		painter->drawImage(rc.topLeft(),img);
	}
*/
}

QIcon BarChart::toolBoxIcon()
{
	return QIcon(":/legend.png");
}

QString BarChart::toolBoxText()
{
	return tr("Bar chart");
}

QString BarChart::toolBoxGroup()
{
	return tr("Charts");
}

QObject * BarChart::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new BarChart(parent, parentObject);
}

int BarChart::barsIdentation()
{
	return m_barsIdentation;
}

void BarChart::setBarsIdentation(int barsIdentation)
{
	if (barsIdentation<1)
		barsIdentation=1;
	m_barsIdentation=barsIdentation;
	update();
}

/*
bool BarChart::show3d()
{
	return m_show3d;
}
void BarChart::setShow3d(bool show3d)
{
	m_show3d=show3d;
	update();
}
*/

bool BarChart::showLabels()
{
	return m_showLabels;
}

void BarChart::setShowLabels(bool showLabels)
{
	m_showLabels=showLabels;
	update();
}

bool BarChart::showGrid()
{
	return m_showGrid;
}
void BarChart::setShowGrid(bool showGrid)
{
	m_showGrid=showGrid;
	update();
}

qreal BarChart::toColorFactor()
{
	return m_toColorFactor;
}

void BarChart::setToColorFactor(qreal toColorFactor)
{
	if (toColorFactor>10)
		toColorFactor=10;

	if (toColorFactor<0.1)
		toColorFactor=0.1;
	m_toColorFactor=toColorFactor;
	update();
}

bool BarChart::drawBorder()
{
	return m_drawBorder;
}
void BarChart::setDrawBorder(bool drawBorder)
{
	m_drawBorder=drawBorder;
	update();
}

Q_EXPORT_PLUGIN2(barChart, BarChart)
