/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by Mikhalov Alexander                              *
 *   alexmi3@rambler.ru                                                    *
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

#include "QtCore"
#include "csvdataset.h"
#include "csvdataseteditor.h"

CsvDataset::CsvDataset(QObject *parent)
 : Report::DataSet(parent)
{
    m_currentRow = 0;
    m_isPopulated = false;
    m_delimeter = ",";
    m_model = new Model (this);
    m_fmodel = new QSortFilterProxyModel(this);
    m_fmodel->setSourceModel(m_model);
    m_fmodel->setDynamicSortFilter ( true );
}


CsvDataset::~CsvDataset()
{
}


Report::DataSet * CsvDataset::createInstance(QObject* parent)
{
    return new CsvDataset(parent);
}

DataSetEditor * CsvDataset::createEditor()
{
    return new CsvDatasetEditor();
}

QString CsvDataset::name()
{
    return QString("CSV Dataset");
}

QString CsvDataset::lastError()
{
    return m_error;
}

QString	CsvDataset::fileName()
{
    return m_fileName;
}

void CsvDataset::setFileName(QString str)
{
    m_fileName = str;
}

QStringList CsvDataset::list()
{
    return m_list;
}

void  CsvDataset::setList(QStringList list)
{
    m_list = list;
}

QString CsvDataset::delimeter ()
{
    return m_delimeter;
}

void CsvDataset::setDelimeter (QString str)
{
    m_delimeter = str;
}

QString CsvDataset::fieldName(int column )
{
    return QString();
    /*
    if (m_isPopulated)
	return m_model.record().fieldName(column);
    else
	return QString();
	*/
}

QAbstractItemModel * CsvDataset::model()
{
    return m_fmodel;
}


bool CsvDataset::populate()
{
    qDebug("m_list count = %i", m_list.count());

    QStringList list = m_list;
    if (!list.count())			//** data not injected direct to report as strinlist
    {
	if (m_fileName.isEmpty())
	{
	    m_error = tr("filename is empty. Please enter filename first");
	    return false;
	}

	QFile data(m_fileName);
	if (data.open(QFile::ReadOnly | QFile::Text)) {
	    QTextStream in(&data);
	    do {
		list.append(in.readLine());
	    } while (!in.atEnd() );
	}
	else
	{
	    m_error = tr("Can't open filename %1").arg(m_fileName);
	    return false;
	}
    }

    Array array;
    for (int i=0; i<list.count(); i++)
	array.append(list.at(i).split(m_delimeter));
    m_model->setArray(array);

    //** header generator
    for (int i=0; i<m_model->columnCount(); i++)
	m_model->setHeaderData(i, Qt::Horizontal, QString("field_%1").arg(i));

    m_isPopulated = true;
    return true;
}

bool CsvDataset::isPopulated()
{
    return m_isPopulated;
}

bool CsvDataset::first()
{
	emit(beforeFirst());
	m_currentRow = 0;
	bool ret = size();
	emit(afterFirst());
	return ret;
}

bool CsvDataset::last()
{
	emit(beforeLast());
	m_currentRow = m_fmodel->rowCount();
	bool ret = m_currentRow < m_fmodel->rowCount() ? true:false;
	emit(afterLast());
	return ret;
}

bool CsvDataset::next()
{
	emit(beforeNext());
	m_currentRow++;
	bool ret = m_currentRow < size();
	emit(afterNext());
	return ret;
}

bool CsvDataset::previous()
{
	emit(beforePrevious());
	m_currentRow--;
	bool ret = m_currentRow >= 0;
	emit(afterPrevious());
	return ret;
}


bool CsvDataset::seek(int index)
{
	emit(beforeSeek(index));
	m_currentRow = index;
	bool ret = (m_currentRow >=0 && m_currentRow < m_fmodel->rowCount() ? true:false);
	emit(afterSeek(index));
	return ret;
}

int CsvDataset::size()
{
	return m_fmodel->rowCount();
}

QVariant CsvDataset::value(int index) const
{
    return m_fmodel->data( m_fmodel->index(m_currentRow,index) );
}


QVariant CsvDataset::value(const QString & field) const
{
    return QVariant();
//    return m_model.record(m_currentRow).value(field);
}

QVariant CsvDataset::lookaheadValue(int index) const
{
     return QVariant();
//    return m_currentRow+1 < m_model.rowCount() && index < m_model.columnCount() ?  m_model.record(m_currentRow + 1).value(index) : QVariant::Invalid;
}

QVariant CsvDataset::lookaheadValue(const QString & field) const
{
     return QVariant();
//    return m_currentRow+1 < m_model.rowCount() ?  m_model.record(m_currentRow + 1).value(field) : QVariant::Invalid;
}

QVariant CsvDataset::lookbackValue(int index) const
{
     return QVariant();
//    return m_currentRow-1 < 0 && index < m_model.columnCount() ?  m_model.record(m_currentRow + 1).value(index) : QVariant::Invalid;
}

QVariant CsvDataset::lookbackValue(const QString & field) const
{
     return QVariant();
//    return m_currentRow-1 < 0  ?  m_model.record(m_currentRow + 1).value(field) : QVariant::Invalid;
}

Q_EXPORT_PLUGIN2(csvDataset, CsvDataset)
