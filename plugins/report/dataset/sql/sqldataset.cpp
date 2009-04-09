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
#include <QTextEdit>

#include "sqldataset.h"
#include "sqldataseteditor.h"

SqlDataset::SqlDataset(QObject *parent)
 : Report::DataSet(parent)
{
    m_currentRow = 0;
    m_isPopulated = false;
}


SqlDataset::~SqlDataset()
{
}


Report::DataSet * SqlDataset::createInstance(QObject* parent)
{
    return new SqlDataset(parent);
}

DataSetEditor * SqlDataset::createEditor()
{
    return new SqlDatasetEditor();
}

QString SqlDataset::name()
{
    return QString("SQL Dataset");
}

QString SqlDataset::lastError()
{
    return m_model.lastError().text();
}

QString SqlDataset::fieldName(int column )
{
    if (m_isPopulated)
	return m_model.record().fieldName(column);
    else
	return QString();
}

QAbstractTableModel * SqlDataset::model()
{
    return &m_model;
}

QString SqlDataset::text()
{
    return m_queryText;
}

void SqlDataset::setText(QString str)
{
    m_queryText = str;
}

bool SqlDataset::populate()
{
    m_model.setQuery(m_queryText);
    bool ret = !m_model.lastError().isValid();
    if ( !QSqlDatabase::database().driver()->hasFeature(QSqlDriver::QuerySize))
	while (m_model.canFetchMore())
	    m_model.fetchMore();
    m_isPopulated = ret;
    return ret;
}

bool SqlDataset::isPopulated()
{
    return m_isPopulated;
}

bool SqlDataset::first()
{
	emit(beforeFirst());
	m_currentRow = 0;
	bool ret = size();
	emit(afterFirst());
	return ret;
}

bool SqlDataset::last()
{
	emit(beforeLast());
	m_currentRow = m_model.rowCount();
	bool ret = !m_model.record(m_currentRow).isEmpty();
	emit(afterLast());
	return ret;
}

bool SqlDataset::next()
{
	emit(beforeNext());
	m_currentRow++;
	bool ret = m_currentRow < size();
	emit(afterNext());
	return ret;
}

bool SqlDataset::previous()
{
	emit(beforePrevious());
	m_currentRow--;
	bool ret = m_currentRow >= 0;
	emit(afterPrevious());
	return ret;
}

/*
bool SqlDataset::prepare(const QString & query)
{
	return QSqlQuery::prepare(query);
}
*/

bool SqlDataset::seek(int index)
{
	emit(beforeSeek(index));
	m_currentRow = index;
	bool ret = !m_model.record(m_currentRow).isEmpty();
	emit(afterSeek(index));
	return ret;
}

int SqlDataset::size()
{
	return m_model.rowCount();
}

QVariant SqlDataset::value(int index) const
{
    return m_model.record(m_currentRow).value(index);
}

QVariant SqlDataset::value(const QString & field) const
{
    return m_model.record(m_currentRow).value(field);
}

QVariant SqlDataset::lookaheadValue(int index) const
{
    return m_currentRow+1 < m_model.rowCount() && index < m_model.columnCount() ?  m_model.record(m_currentRow + 1).value(index) : QVariant::Invalid;
}

QVariant SqlDataset::lookaheadValue(const QString & field) const
{
    return m_currentRow+1 < m_model.rowCount() ?  m_model.record(m_currentRow + 1).value(field) : QVariant::Invalid;
}

QVariant SqlDataset::lookbackValue(int index) const
{
    return m_currentRow-1 < 0 && index < m_model.columnCount() ?  m_model.record(m_currentRow + 1).value(index) : QVariant::Invalid;
}

QVariant SqlDataset::lookbackValue(const QString & field) const
{
    return m_currentRow-1 < 0  ?  m_model.record(m_currentRow + 1).value(field) : QVariant::Invalid;
}

Q_EXPORT_PLUGIN2(sqlDataset, SqlDataset)
