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

#ifndef REPORTCHARTDATACOLLECTOR_H
#define REPORTCHARTDATACOLLECTOR_H

#include <iteminterface.h>
#include "chartinterface.h"

namespace Report
{

/**
	@author BogDan Vatra <taipan@omnidatagrup.ro>
*/

class ChartDataCollector: public Report::ItemInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);
	Q_PROPERTY(int chartColorOpacity READ colorOpacity WRITE setColorOpacity )
	Q_PROPERTY(ChartDataSource chartDataSource READ chartDataSource WRITE setChartDataSource)

	Q_PROPERTY(QStringList staticData READ staticData WRITE setStaticData)
	Q_PROPERTY(QString query READ query WRITE setQuery)

	Q_PROPERTY(QString keyScript READ keyScript WRITE setKeyScript) // we can collect data from scripts
	Q_PROPERTY(QString valueScript READ valueScript WRITE setValueScript)
	Q_PROPERTY(QString colorScript READ colorScript WRITE setColorScript)

	Q_PROPERTY(QString keyField READ keyField WRITE setKeyField) // or from fields
	Q_PROPERTY(QString valueField READ valueField WRITE setValueField)
	Q_PROPERTY(QString colorField READ colorField WRITE setColorField)

	Q_PROPERTY(int showOnlyFirstValues READ showOnlyFirstValues WRITE setShowOnlyFirstValues)
	Q_PROPERTY(bool showOtherValue READ showOtherValue WRITE setShowOtherValue)
	Q_PROPERTY(QString otherKey READ otherKey WRITE setOtherKey)
	Q_PROPERTY(SortDirection sortDirection READ sortDirection WRITE setSortDirection)

	Q_ENUMS(ChartDataSource SortDirection)

public:
	enum ChartDataSource{FromStaticData, FromDatabase, FromScript};
	enum SortDirection {Unsorted, Ascendent, Descendent};

public:
	ChartDataCollector(QGraphicsItem* parent = 0, QObject* parentObject = 0);
	~ChartDataCollector();

	int colorOpacity();
	void setColorOpacity(int colorOpacity);

	ChartDataSource chartDataSource();
	void setChartDataSource(ChartDataSource chartDataSource);

	QStringList staticData();
	void setStaticData(const QStringList &staticData);

	QString query();
	void setQuery(QString query);

	QString keyScript();
	void setKeyScript(const QString & keyScript);

	QString valueScript();
	void setValueScript(const QString & valueScript);

	QString colorScript();
	void setColorScript(const QString & colorScript);

	QString keyField();
	void setKeyField(const QString & keyField);

	QString valueField();
	void setValueField(const QString & valueField);

	QString colorField();
	void setColorField(const QString & colorField);

	int showOnlyFirstValues();
	void setShowOnlyFirstValues(int showOnlyFirstValues);

	bool showOtherValue();
	void setShowOtherValue(bool showOtherValue);

	QString otherKey();
	void setOtherKey(const QString & otherKey);

	SortDirection sortDirection();
	void setSortDirection(SortDirection sortDirection);

	/**
	 * Returns chart values
	 * @return chart values
	 */
	QList<Report::ChartInterface::_chartValue> values();

	virtual QColor generateNextColor();

protected:
	void addChartValue(const Report::ChartInterface::_chartValue & value);

public slots:
	void clear();
	void collectData();
	void setupConnections();

private:
	QStringList m_staticData;
	ChartDataSource m_chartDataSource;
	QString m_keyScript;
	QString m_valueScript;
	QString m_colorScript;
	QString m_query;
	QString m_keyField;
	QString m_valueField;
	QString m_colorField;
	QList <Report::ChartInterface::_chartValue> m_chartValues;
	Report::ChartInterface::_chartValue m_otherValue;
	int m_showOnlyFirstValues;
	bool m_showOtherValue;
	SortDirection m_sortDirection;
	int m_colorOpacity;

private:
	void collectStaticData();
	static bool compChart(Report::ChartInterface::_chartValue a, Report::ChartInterface::_chartValue b);
	static bool compChartD(Report::ChartInterface::_chartValue a, Report::ChartInterface::_chartValue b);
};

}

Q_DECLARE_INTERFACE(Report::ChartDataCollector, "ro.bigendian.Report.ChartDataCollector/1.0");

#endif
