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

#include <QSqlRecord>
#include <QSqlField>

#include <sqlquery.h>
#include <reportinterface.h>
#include "chartdatacollector.h"

namespace Report {

ChartDataCollector::ChartDataCollector(QGraphicsItem* parentItem, QObject* parentObject)
 : Report::ItemInterface(parentItem, parentObject),m_chartDataSource(FromDatabase),m_showOnlyFirstValues(-1), m_sortDirection(Unsorted),m_colorOpacity(100)
{
	m_otherValue.value=0;
	m_otherValue.key=tr("Other");
	if (reportObject())
		connect(reportObject(),SIGNAL(beforeExec()),SLOT(setupConnections()));
}


ChartDataCollector::~ChartDataCollector()
{
}

ChartDataCollector::ChartDataSource ChartDataCollector::chartDataSource()
{
	return m_chartDataSource;
}
void ChartDataCollector::setChartDataSource(ChartDataSource chartDataSource)
{
	m_chartDataSource=chartDataSource;
	collectStaticData();
}

QStringList ChartDataCollector::staticData()
{
	return m_staticData;
}
void ChartDataCollector::setStaticData(const QStringList &staticData)
{
	m_staticData=staticData;
	collectStaticData();
}

QString ChartDataCollector::query()
{
	return m_query;
}
void ChartDataCollector::setQuery(QString query)
{
	m_query=query;
}

QString ChartDataCollector::keyScript()
{
	return m_keyScript;
}
void ChartDataCollector::setKeyScript(const QString & keyScript)
{
	m_keyScript=keyScript;
}

QString ChartDataCollector::valueScript()
{
	return m_valueScript;
}
void ChartDataCollector::setValueScript(const QString & valueScript)
{
	m_valueScript=valueScript;
}

QString ChartDataCollector::colorScript()
{
	return m_colorScript;
}
void ChartDataCollector::setColorScript(const QString & colorScript)
{
	m_colorScript=colorScript;
}

QString ChartDataCollector::keyField()
{
	return m_keyField;
}
void ChartDataCollector::setKeyField(const QString &keyField)
{
	m_keyField=keyField;
}

QString ChartDataCollector::valueField()
{
	return m_valueField;
}
void ChartDataCollector::setValueField(const QString & valueField)
{
	m_valueField=valueField;
}

QString ChartDataCollector::colorField()
{
	return m_colorField;
}
void ChartDataCollector::setColorField(const QString &colorField)
{
	m_colorField=colorField;
}

int ChartDataCollector::showOnlyFirstValues()
{
	return m_showOnlyFirstValues;
}
void ChartDataCollector::setShowOnlyFirstValues(int showOnlyFirstValues)
{
	if (showOnlyFirstValues<1)
		m_showOnlyFirstValues=-1;
	m_showOnlyFirstValues=showOnlyFirstValues;
	collectStaticData();
}

bool ChartDataCollector::showOtherValue()
{
	return m_showOtherValue;
}
void ChartDataCollector::setShowOtherValue(bool showOtherValue)
{
	m_showOtherValue=showOtherValue;
	collectStaticData();
}

QString ChartDataCollector::otherKey()
{
	return m_otherValue.key;
}

void ChartDataCollector::setOtherKey(const QString &otherKey)
{
	m_otherValue.key=otherKey;
	update();
}

ChartDataCollector::SortDirection ChartDataCollector::sortDirection()
{
	return m_sortDirection;
}
void ChartDataCollector::setSortDirection(SortDirection sortDirection)
{
	m_sortDirection=sortDirection;
	collectStaticData();
}

QColor ChartDataCollector::generateNextColor()
{
	return QColor(qrand()%255,qrand()%255,qrand()%255,255);
}

void ChartDataCollector::setupConnections()
{
	m_chartValues.clear();
	m_otherValue.value=0;
	if (m_chartDataSource!=FromStaticData && m_query.length() && findQuery(m_query))
	{
		findQuery(m_query)->disconnect();
		connect(findQuery(m_query),SIGNAL(afterFirst()),SLOT(collectData()));
		connect(findQuery(m_query),SIGNAL(afterNext()),SLOT(collectData()));
		connect(findQuery(m_query),SIGNAL(afterPrevious()),SLOT(collectData()));
		connect(findQuery(m_query),SIGNAL(afterLast()),SLOT(collectData()));
	}
}
void ChartDataCollector::addChartValue(const Report::ChartInterface::_chartValue & value)
{
	if (m_showOnlyFirstValues<1 || m_showOnlyFirstValues>m_chartValues.size())
		m_chartValues.push_back(value);
	else
	{
		int pos=-1;
		for (int i=0;i<m_chartValues.size();i++)
			if (m_chartValues[i].value<value.value)
			{
				if (-1==pos)
					pos=i;
				else
					if (m_chartValues[i].value<m_chartValues[pos].value)
						pos=i;
			}
		if (-1==pos)
			m_otherValue.value+=value.value;
		else
		{
			m_otherValue.value+=m_chartValues[pos].value;
			m_chartValues[pos]=value;
		}
	}	
	m_otherValue.color=generateNextColor();
}
void ChartDataCollector::clear()
{
	m_chartValues.clear();
}

void ChartDataCollector::collectData()
{
	SqlQuery * qry=findQuery(m_query);
	if (!qry)
		return;

	Report::ChartInterface::_chartValue cv;
	bool ok;
	switch(m_chartDataSource)
	{
		case FromDatabase:
			if(!qry->record().field(m_valueField).isValid())
				break;

			if(qry->record().field(m_keyField).isValid())
				cv.key=qry->record().field(m_keyField).value().toString();

			if(qry->record().field(m_colorField).isValid())
				cv.color=qry->record().field(m_keyField).value().value<QColor>();
			else
				cv.color=generateNextColor();

			cv.value=qry->record().field(m_valueField).value().toDouble(&ok);
			if (ok)
				addChartValue(cv);
			break;

		case FromScript:
			if (!m_valueScript.length() || !scriptEngine())
				break;
			cv.value=scriptEngine()->evaluate(m_valueScript).toVariant().toDouble(&ok);
			if (!ok)
				break;
			if (m_keyScript.length())
				cv.key=scriptEngine()->evaluate(m_keyScript).toString();

			if (m_colorScript.length())
				cv.color=scriptEngine()->evaluate(m_colorScript).toVariant().value<QColor>();
			else
				cv.color=generateNextColor();
			addChartValue(cv);
			break;
		default:
			break;
	}
}

QList<Report::ChartInterface::_chartValue> ChartDataCollector::values()
{
	QList<Report::ChartInterface::_chartValue> vl=m_chartValues;
	if (m_showOtherValue && m_otherValue.value)
		vl.push_back(m_otherValue);

	if (m_sortDirection==Ascendent)
		qSort(vl.begin(),vl.end(), compChart);
	if (m_sortDirection==Descendent)
		qSort(vl.begin(),vl.end(),  compChartD);

	for(int i=0;i<vl.size();i++)
		vl[i].color.setAlpha(255.*(m_colorOpacity/100.));

	return vl;
}

void ChartDataCollector::collectStaticData()
{
	m_chartValues.clear();
	m_otherValue.value=0;

	if (m_chartDataSource!=FromStaticData)
		return;	

	foreach(QString line, m_staticData)
	{
		QStringList chart=line.split(',');
		if (!chart.size())
			continue;
		Report::ChartInterface::_chartValue cv;
		bool ok;
		cv.value=chart[0].toDouble(&ok);
		if (!ok)
			continue;

		if (chart.size()>=2)
			cv.key=chart[1];

		if (3==chart.size())
			cv.color=QVariant(chart[2]).value<QColor>();
		else
			cv.color=generateNextColor();
		addChartValue(cv);
	}
	update();
}

bool ChartDataCollector::compChart(Report::ChartInterface::_chartValue a, Report::ChartInterface::_chartValue b)
{
	return a.value<b.value;
}

bool ChartDataCollector::compChartD(Report::ChartInterface::_chartValue a, Report::ChartInterface::_chartValue b)
{
	return a.value>b.value;
}

int ChartDataCollector::colorOpacity()
{
	return m_colorOpacity;
}
void ChartDataCollector::setColorOpacity(int colorOpacity)
{
	if (colorOpacity < 1)
		colorOpacity = 1;
	if (colorOpacity > 100)
		colorOpacity = 100;
	m_colorOpacity =  colorOpacity;
	update();
}
}
