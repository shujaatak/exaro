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

#include "propertyinterface.h"
#include "propertymodel.h"
#include <QMetaProperty>
#include <QMetaEnum>

namespace PropertyEditor
{

PropertyInterface::PropertyInterface(QObject *parent , QObject * object, int property, const PropertyModel * /*pm*/): QObject(parent)
{
	setObject(object);
	setObjectProperty(property);
	m_properties.clear();
}

void PropertyInterface::refreshPropertis(const PropertyModel * pm)
{
	if (!pm)
		return;

	for (int i = 0;i < m_properties.size();i++)
		delete m_properties[i];

	m_properties.clear();

	for (int i = 0;i < metaObject()->propertyCount();i++)
		if (metaObject()->property(i).isUser() && metaObject()->property(i).isValid() && metaObject()->property(i).isWritable() && metaObject()->property(i).isReadable())
		{
			PropertyInterface* pi = pm->createPropery(this, i);
			if (pi)
				m_properties.push_back(pi);
		}
}
void PropertyInterface::setObject(QObject * object)
{
	m_object = object;
}

QVariant PropertyInterface::value()
{
	return object()->property(object()->metaObject()->property(objectProperty()).name());

}

bool PropertyInterface::setValue(QVariant data)
{
	return object()->setProperty(object()->metaObject()->property(objectProperty()).name(), data);
}

QObject * PropertyInterface::object()
{
	return m_object;
}

void PropertyInterface::setObjectProperty(int property)
{
	m_property = property;
}

int PropertyInterface::objectProperty()
{
	return m_property;
}

PropertyInterface::~PropertyInterface()
{
	for (int p = 0;p < m_properties.size();p++)
		delete m_properties[p];
}

int PropertyInterface::rowCount(const QModelIndex & parent)
{
	if (!parent.isValid() || parent.column())
		return 0;
	return m_properties.size(); // numarul de propietati
}

int PropertyInterface::columnCount(const QModelIndex & parent)
{
	if (!parent.column() && m_properties.size()) // daca este pe coloana 0 si am propietati atunci o sa am 2 coloane
		return 2;
	return 0;
}


void PropertyInterface::setParentIndex(const QModelIndex & index)
{
	m_parentIndex = index;
}

QModelIndex PropertyInterface::parentIndex(const QModelIndex & index)
{
	Q_UNUSED(index);
	return m_parentIndex;
}

Qt::ItemFlags PropertyInterface::flags(const QModelIndex &index)
{
	if (index.column())
		return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
	else
		return Qt::ItemIsEnabled;
}

PropertyInterface* PropertyInterface::property(int prop) const
{
	if (m_properties.size() <= prop)
		return 0;
	return m_properties[prop];
}

}

