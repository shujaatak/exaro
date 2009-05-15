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

#ifndef DATASET_H
#define DATASET_H

#include <QAbstractTableModel>
#include <QObject>
#include <QVariant>
#include <QStringList>

class DataSetEditor;

namespace Report
{
class DataSet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString parentDataset READ parentDataset WRITE setParentDataset)
    Q_PROPERTY(QString filterCondition READ filterCondition WRITE setFilterCondition)
    Q_PROPERTY(int filterColumn READ filterColumn WRITE setFilterColumn)

public:
	DataSet(QObject *parent = 0);
	~DataSet();

	Q_INVOKABLE virtual bool first();
	Q_INVOKABLE virtual bool last();
	Q_INVOKABLE virtual bool next();
	Q_INVOKABLE virtual bool previous();
	Q_INVOKABLE virtual bool populate();
	Q_INVOKABLE virtual bool isPopulated();
	Q_INVOKABLE virtual bool seek(int index);
	Q_INVOKABLE virtual int size();
	Q_INVOKABLE virtual QVariant value(int index) const;
	Q_INVOKABLE virtual QVariant value(const QString & field) const;
	Q_INVOKABLE virtual QVariant lookaheadValue(int index) const;
	Q_INVOKABLE virtual QVariant lookaheadValue(const QString & field) const;
	Q_INVOKABLE virtual QVariant lookbackValue(int index) const;
	Q_INVOKABLE virtual QVariant lookbackValue(const QString & field) const;
	virtual DataSet * createInstance(QObject* parent = 0);
	virtual DataSetEditor * createEditor();
	Q_INVOKABLE virtual QAbstractItemModel * model();
	Q_INVOKABLE virtual QString name();
	Q_INVOKABLE virtual QString lastError();
	Q_INVOKABLE virtual QString fieldName(int column );
	Q_INVOKABLE virtual void setFilter ( const int col, const QString & str, Qt::CaseSensitivity cs = Qt::CaseSensitive );

	QString	    parentDataset();
	void	    setParentDataset(QString pDataset);
	QString	    filterCondition();
	void	    setFilterCondition(QString str);
	int	    filterColumn();
	void	    setFilterColumn(int col);

signals:
	void beforeNext();
	void afterNext();
	void beforePrevious();
	void afterPrevious();
	void beforeFirst();
	void afterFirst();
	void beforeLast();
	void afterLast();
	void beforeSeek(int index);
	void afterSeek(int index);

private:
	QString m_parentDataset;
	QString m_filterCondition;
	int m_filterColumn;

};
}
Q_DECLARE_INTERFACE(Report::DataSet, "ro.bigendian.ReportDesigner.DataSet/1.0");

#endif
