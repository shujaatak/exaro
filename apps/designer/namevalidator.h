/***************************************************************************
 *   Copyright (C) 2009 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
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
#ifndef NAMEVALIDATOR_H
#define NAMEVALIDATOR_H

#include <propertyvalidator.h>

class NameValidator: public PropertyEditor::PropertyValidator
{
	Q_OBJECT

public:

	NameValidator(QObject *parent = 0);
	void setObject(QObject * object);
	void setProperty(const QString & property);
	void setRootObject(QObject * obj);
	void fixup ( QString & input ) const;
	State validate ( QString & input, int & pos ) const;
private:
	bool isUnique(QObject * object, const QString & name) const;
	void makeUnique(QString & name) const;
private:
	QObject * m_obj;
	QObject * m_rootObj;
	QString  m_property;
};

#endif
