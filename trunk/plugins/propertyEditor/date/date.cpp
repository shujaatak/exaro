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

#include <QtCore>
#include <QMetaProperty>
#include <QMetaEnum>
#include <QDateEdit>

#include "date.h"

Date::Date(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
}

QWidget* Date::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	QDateEdit *de = new QDateEdit(parent);
	de->setDate(value().toDate());
	connect(de, SIGNAL(dateChanged(const QDate&)), this, SLOT(setValue(const QDate&)));
	return de;
}

void Date::setValue(const QDate& d)
{
	PropertyInterface::setValue(d);
}

QVariant Date::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || -1 == objectProperty())
		return QVariant();
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return value().toDate().toString();
	}
	return QVariant();
}

bool Date::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}

void Date::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
	dynamic_cast<QDateEdit *>(editor)->setDate(value().toDate());
}


bool Date::canHandle(QObject * object, int property)const
{
	if (object->metaObject()->property(property).isEnumType() || object->metaObject()->property(property).isFlagType())
		return false;

	switch (object->property(object->metaObject()->property(property).name()).type())
	{
		case QVariant::Date:
			return true;
		default:
			return false;
	}
}

PropertyInterface* Date::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
	return new Date(parent(), object, property, propertyModel);
}

Q_EXPORT_PLUGIN2(date, Date);
