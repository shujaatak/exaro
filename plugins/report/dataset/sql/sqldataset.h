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

#ifndef SQLDATASET_H
#define SQLDATASET_H

#include <dataset.h>
#include <dataseteditor.h>
#include <QtSql>

class SqlDataset : public Report::DataSet
{
	Q_OBJECT
	Q_INTERFACES(Report::DataSet);
	Q_PROPERTY(QString text READ text WRITE setText)

public:
	SqlDataset(QObject *parent = 0);
	~SqlDataset();

	DataSet * createInstance(QObject* parent = 0);
	DataSetEditor * createEditor();
	QAbstractTableModel * model();
	QString name();
	QString lastError();

	bool first();
	bool last();
	bool next();
	bool previous();
	bool populate();
	bool isPopulated();
	bool seek(int index);
	int size();
	QVariant value(int index) const;
	QVariant value(const QString & field) const;
	QVariant lookaheadValue(int index) const;
	QVariant lookaheadValue(const QString & field) const;
	QVariant lookbackValue(int index) const;
	QVariant lookbackValue(const QString & field) const;
	QString fieldName(int column );

	QString	    text();
	void	    setText(QString str);

private:
	int m_currentRow;
	bool m_isPopulated;
	QString m_queryText;
	QSqlQueryModel m_model;
	DataSetEditor ed;
};

#endif
