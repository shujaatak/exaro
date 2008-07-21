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
#include <QComboBox>

#include "enums.h"

Enums::Enums(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
}

QWidget* Enums::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	cb = new QComboBox(parent);
	for (int en = 0;en < object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).keyCount();en++)
		cb->insertItem(en, object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).key(en), object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).value(en));

	cb->findData(value().toInt());
	connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
	return cb;
}

void Enums::setValue(int item)
{
	PropertyInterface::setValue(cb->itemData(item));
}

QVariant Enums::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || -1 == objectProperty())
		return QVariant();
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return object()->metaObject()->enumerator(object()->metaObject()->indexOfEnumerator(object()->metaObject()->property(objectProperty()).typeName())).valueToKey(value().toInt());
	}
	return QVariant();
}

bool Enums::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}

void Enums::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
	dynamic_cast<QComboBox *>(editor)->setCurrentIndex(dynamic_cast<QComboBox *>(editor)->findData(value().toInt()));
}


bool Enums::canHandle(QObject * object, int property)const
{
	if (object->metaObject()->property(property).isEnumType() && !object->metaObject()->property(property).isFlagType())
		return true;
	return false;
}

PropertyInterface* Enums::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
	return new Enums(parent(), object, property, propertyModel);
}

Q_EXPORT_PLUGIN2(enums, Enums);
