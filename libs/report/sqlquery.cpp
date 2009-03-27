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

#include <QtSql>
#include <QSqlRecord>
#include <QSqlDatabase>

#include "sqlquery.h"
namespace Report
{
DataSet::DataSet(QObject *parent)
		: QSqlQueryModel(parent)
{
    m_currentRow = 0;
    m_isPopulated = false;
}


DataSet::~DataSet()
{
}

QString DataSet::text()
{
    return m_queryText;
}

void DataSet::setText(QString str)
{
    m_queryText = str;
}


QString DataSet::parentQuery()
{
    return m_parentQuery;
}

void DataSet::setParentQuery(QString pQuery)
{
    m_parentQuery = pQuery;
}

void DataSet::setParentCondition(QStringList list)
{
    m_parentCondition = list;
}

QStringList DataSet::parentCondition()
{
    return m_parentCondition;
}

bool DataSet::populate()
{
    setQuery(m_queryText);
    bool ret = !lastError().isValid();
    if ( !QSqlDatabase::database().driver()->hasFeature(QSqlDriver::QuerySize))
	while (canFetchMore())
	    fetchMore();
    m_isPopulated = ret;
    return ret;
}

bool DataSet::populate(const QString & query)
{
    setText(query);
    return populate();
}

bool DataSet::isPopulated()
{
    return m_isPopulated;
}

bool DataSet::first()
{
	emit(beforeFirst());
	m_currentRow = 0;
	bool ret = size();
	emit(afterFirst());
	return ret;
}

bool DataSet::last()
{
	emit(beforeLast());
	m_currentRow = rowCount();
	bool ret = !record(m_currentRow).isEmpty();
	emit(afterLast());
	return ret;
}

bool DataSet::next()
{
	emit(beforeNext());
	m_currentRow++;
	bool ret = m_currentRow < size();
	emit(afterNext());
	return ret;
}

bool DataSet::previous()
{
	emit(beforePrevious());
	m_currentRow--;
	bool ret = m_currentRow >= 0;
	emit(afterPrevious());
	return ret;
}

/*
bool DataSet::prepare(const QString & query)
{
	return QSqlQuery::prepare(query);
}
*/

bool DataSet::seek(int index)
{
	emit(beforeSeek(index));
	m_currentRow = index;
	bool ret = !record(m_currentRow).isEmpty();
	emit(afterSeek(index));
	return ret;
}

int DataSet::size()
{
	return rowCount();
}

QVariant DataSet::value(int index) const
{
    return record(m_currentRow).value(index);
}

QVariant DataSet::value(const QString & field) const
{
    return record(m_currentRow).value(field);
}

QVariant DataSet::lookaheadValue(int index) const
{
    return m_currentRow+1 < rowCount() && index < columnCount() ?  record(m_currentRow + 1).value(index) : QVariant::Invalid;
}

QVariant DataSet::lookaheadValue(const QString & field) const
{
    return m_currentRow+1 < rowCount() ?  record(m_currentRow + 1).value(field) : QVariant::Invalid;
}

QVariant DataSet::lookbackValue(int index) const
{
    return m_currentRow-1 < 0 && index < columnCount() ?  record(m_currentRow + 1).value(index) : QVariant::Invalid;
}

QVariant DataSet::lookbackValue(const QString & field) const
{
    return m_currentRow-1 < 0  ?  record(m_currentRow + 1).value(field) : QVariant::Invalid;
}


}
