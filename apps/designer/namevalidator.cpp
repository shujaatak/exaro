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
#include "namevalidator.h"

NameValidator::NameValidator(QObject *parent): PropertyEditor::PropertyValidator(parent)
{
	m_obj=m_rootObj=0;
}

void NameValidator::setObject(QObject* object) 
{
	m_obj=object;
}

void NameValidator::setProperty(const QString& property) 
{
	m_property=property;
}

void NameValidator::setRootObject(QObject* obj) 
{
	m_rootObj=obj;
}

QValidator::State NameValidator::validate(QString& input, int& pos) const 
{
	if (m_property=="objectName" && m_rootObj && m_obj)
	{
		if (isUnique(m_rootObj, input))
			return QValidator::Acceptable;
		else 
		{
			return QValidator::Intermediate;
		}
	}
	return QValidator::Acceptable;
}

void NameValidator::fixup(QString& input) const 
{
	makeUnique(input);
}

void NameValidator::makeUnique(QString & name) const
{
	for (int i = 0;i < name.size();i++)
		if (name[i] < 'a' && name[i] > 'z' && name[i] < '0' && name[i] > '9')
			name[i] = '_';

	if (isUnique(m_rootObj, name))
		return;

	int number = 1;
	if (!name.endsWith("_"))
		name += "_";
	forever
	{
		if (isUnique(m_rootObj, name+QString::number(number, 10)))
		{
			name += QString::number(number, 10);
			break;
		}
		number++;
	}
}

bool NameValidator::isUnique(QObject * object, const QString & name ) const
{
	if (!object)
		return true;

	if (object->objectName() == name && m_obj!=object)
		return false;

	for (int i = 0;i < object->children().size();i++)
		if (!isUnique(object->children()[i], name))
			return false;
	return true;
}

