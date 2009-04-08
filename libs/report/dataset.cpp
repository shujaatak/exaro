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

#include "dataset.h"
#include "dataseteditor.h"

namespace Report
{
DataSet::DataSet(QObject *parent)
		: QObject(parent)
{
}


DataSet::~DataSet()
{
}


QString	DataSet::parentDataset()
{
    return m_parentDataset;
}

void DataSet::setParentDataset(QString pDataset)
{
    m_parentDataset = pDataset;
}

void DataSet::setParentCondition(QStringList list)
{
    m_parentCondition = list;
}

QStringList DataSet::parentCondition()
{
    return m_parentCondition;
}


bool DataSet::first() {return false;}
bool DataSet::last(){return false;}
bool DataSet::next(){return false;}
bool DataSet::previous(){return false;}
bool DataSet::populate(){return false;}
bool DataSet::isPopulated(){return false;}
bool DataSet::seek(int index){return false;}
int DataSet::size(){return 0;}
QVariant DataSet::value(int index) const{return QVariant();}
QVariant DataSet::value(const QString & field) const{return QVariant();}
QVariant DataSet::lookaheadValue(int index) const{return QVariant();}
QVariant DataSet::lookaheadValue(const QString & field) const{return QVariant();}
QVariant DataSet::lookbackValue(int index) const{return QVariant();}
QVariant DataSet::lookbackValue(const QString & field) const{return QVariant();}
DataSet * DataSet::createInstance(QObject* parent){return new DataSet(parent);}
DataSetEditor * DataSet::createEditor() {return 0;}
QAbstractTableModel * DataSet::model() {return 0;}
QString DataSet::name() { return tr("Unknown"); }
QString DataSet::lastError() {return QString();}
QString DataSet::fieldName(int column ) {return tr("Unknown");}

}
