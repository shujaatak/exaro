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

#include <QObject>
#include <QSqlQueryModel>
#include <QVariant>
#include <QStringList>

namespace Report
{
class DataSet : public QSqlQueryModel
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QString parentQuery READ parentQuery WRITE setParentQuery)
    Q_PROPERTY(QStringList parentCondition READ parentCondition WRITE setParentCondition)

public:
	DataSet(QObject *parent = 0);
	~DataSet();

//	Q_INVOKABLE bool exec();
//	Q_INVOKABLE bool exec(const QString & query);
//	Q_INVOKABLE void bindValue(const QString & placeholder, const QVariant & val, QSql::ParamType paramType = QSql::In);
//	Q_INVOKABLE void bindValue(int pos, const QVariant & val, QSql::ParamType paramType = QSql::In);
	Q_INVOKABLE bool first();
	Q_INVOKABLE bool last();
	Q_INVOKABLE bool next();
	Q_INVOKABLE bool previous();
	Q_INVOKABLE bool populate();
	Q_INVOKABLE bool populate(const QString & query);
	Q_INVOKABLE bool isPopulated();
//	Q_INVOKABLE bool prepare(const QString & query);
	Q_INVOKABLE bool seek(int index);
	Q_INVOKABLE int size();
	Q_INVOKABLE QVariant value(int index) const;
	Q_INVOKABLE QVariant value(const QString & field) const;
	Q_INVOKABLE QVariant lookaheadValue(int index) const;
	Q_INVOKABLE QVariant lookaheadValue(const QString & field) const;
	Q_INVOKABLE QVariant lookbackValue(int index) const;
	Q_INVOKABLE QVariant lookbackValue(const QString & field) const;

	QString	    parentQuery();
	void	    setParentQuery(QString pQuery);
	QStringList parentCondition();
	void	    setParentCondition(QStringList str);
	QString	    text();
	void	    setText(QString str);

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
	QString m_parentQuery;
	QStringList m_parentCondition;
	QString m_queryText;
	int m_currentRow;
	bool m_isPopulated;

};
}
#endif
