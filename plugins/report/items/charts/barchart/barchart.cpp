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
#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QLinearGradient>

#include "barchart.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(barchart);
}

BarChart::BarChart(QGraphicsItem* parent, QObject* parentObject) : ChartInterface(parent, parentObject), m_barsIdentation(1/UNIT), m_showLabels(false),m_toColorFactor(1),m_drawBorder(false)
{
	initMyResource();
}

QRectF BarChart::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void BarChart::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit beforePrint(this);

	painter->save();
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;
	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);
	adjustRect(rect);

	QList<ChartInterface::_chartValue> val=values();
	if (!val.size())
	{
		if (option->type != QStyleOption::SO_GraphicsItem)
			emit afterPrint(this);
		painter->restore();
		return;
	}

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
	qreal powVal=(absMaxVal<1)?pow(10,QString::number(absMaxVal).right(QString::number(absMaxVal).indexOf('.')).length()+1):1;
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

	qreal chartStep=pow(10,(QString::number(absMaxVal).left(QString::number(absMaxVal).indexOf('.')).length())-1)/f;
	qreal powStep=(chartStep<1)?10:1;
	chartStep*=powStep;
	maxpv*=powStep;
	minnv*=powStep;
	powVal*=powStep;

	maxpv=maxpv+((((quint64)maxpv%(quint64)chartStep)?((quint64)chartStep-((quint64)maxpv%(quint64)chartStep)):0))/powVal;
	minnv=minnv-(((-(quint64)minnv%(quint64)chartStep)?((quint64)chartStep-(-(quint64)minnv%(quint64)chartStep)):0))/powVal;
	quint64 maxVal=maxpv-minnv;

	if (m_showLabels)
		rc=rc.adjusted(0,(double)painter->fontMetrics().height()/2,0,-(double)painter->fontMetrics().height()/2);

	qreal maxHeight=rc.height();
	qreal valstep=maxHeight/(maxVal/chartStep);

	if (valstep<painter->fontMetrics().height())
	{
		chartStep*=(((quint64)(painter->fontMetrics().height()/valstep))+1);
		valstep=(((quint64)(painter->fontMetrics().height()/valstep))+1)*valstep;
	}

	if (m_showLabels)
	{
#warning FIXME ensure max positive and max negative is visible
		qreal maxLabelWidth=0;
		for(int i=0;i<maxVal/chartStep+1+((quint64)maxVal%(quint64)chartStep?1:0);i++)
			if (maxLabelWidth<painter->fontMetrics().width(QString::number((maxVal*i-chartStep*i)/powVal)))
				maxLabelWidth=painter->fontMetrics().width(QString::number((maxVal*i-chartStep*i)/powVal));
		int y=0;
		for(int i=0;i<maxVal/chartStep+1+((quint64)maxVal%(quint64)chartStep?1:0);i++)
		{
			painter->drawText(QRectF(0,y,maxLabelWidth,painter->fontMetrics().height()),Qt::AlignVCenter|Qt::AlignRight,QString::number((maxpv-chartStep*i)/powVal));
			y+=valstep;
		}

		painter->drawLine(rc.x()+maxLabelWidth+1,0,rc.x()+maxLabelWidth+1,rect.height());
		rc=rc.adjusted(maxLabelWidth+1,0,0,0);
	}
	painter->translate(rc.topLeft());
	if (m_showGrid)
	{
		int y=0;
		for(int i=0;i<maxVal/chartStep+1+((quint64)maxVal%(quint64)chartStep?1:0);i++)
		{
			painter->drawLine(0,y,rc.width(),y);
			y+=valstep;
		}
	}

	int x=m_barsIdentation;
	qreal barWidth=(rc.width()-m_barsIdentation*(val.size()+1))/val.size();
	qreal py=maxHeight/maxVal;
	foreach(ChartInterface::_chartValue cv, val)
	{
		QLinearGradient lg(QPointF(x+barWidth/2,0), QPointF(x+barWidth,0));
		lg.setSpread(QGradient::ReflectSpread);
		lg.setColorAt(0, cv.color);
		lg.setColorAt(1, QColor(cv.color.red()*m_toColorFactor, cv.color.green()*m_toColorFactor, cv.color.blue()*m_toColorFactor, cv.color.alpha()));
		painter->fillRect(QRectF(x,py*maxpv-py*cv.value*powVal,barWidth, py*cv.value*powVal),QBrush(lg));
		if (m_showLabels)
			painter->drawText(QRectF(x-m_barsIdentation/2, py*maxpv-((cv.value>=0)?painter->fontMetrics().height():0), barWidth+m_barsIdentation, painter->fontMetrics().height()),Qt::AlignCenter,QString("%1").arg(cv.value));
		x+=barWidth+m_barsIdentation;
	}

	painter->restore();
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit afterPrint(this);
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

Q_EXPORT_PLUGIN2(legend, BarChart)
