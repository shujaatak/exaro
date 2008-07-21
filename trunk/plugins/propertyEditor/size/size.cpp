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
#include "size.h"

Size::Size(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
	refreshPropertis(propertyModel);
}


QWidget* Size::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	Q_UNUSED(parent);
	return 0;
}

QVariant Size::data(const QModelIndex & index)
{
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return QString("[%1,%2]").arg(value().toSize().width()).arg(value().toSize().height());
	}
	return QVariant();
}

bool Size::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(data);
	Q_UNUSED(index);
	return false;
}

bool Size::canHandle(QObject * object, int property) const
{
	if (object->metaObject()->property(property).isEnumType() || object->metaObject()->property(property).isFlagType())
		return false;

	switch (object->property(object->metaObject()->property(property).name()).type())
	{
		case QVariant::Size:
			return true;
		default:
			return false;
	}
	return false;
}

int Size::height()
{
	return value().toSize().height();
}

void Size::setHeight(int height)
{
	QSize s = value().toSize();
	s.setHeight(height);
	setValue(s);
}

int Size::width()
{
	return value().toSize().width();
}

void Size::setWidth(int width)
{
	QSize s = value().toSize();
	s.setWidth(width);
	setValue(s);
}

Qt::ItemFlags Size::flags(const QModelIndex &/*index*/)
{
	return Qt::ItemIsEnabled;
}

PropertyInterface* Size::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
	return new Size(parent(), object, property, propertyModel);
}

Q_EXPORT_PLUGIN2(size, Size);
