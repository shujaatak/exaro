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

#ifndef PROPERTYEDITORPROPERTYEDITOR_H
#define PROPERTYEDITORPROPERTYEDITOR_H

#include <QTreeView>
#include <QList>


#ifdef WIN32
#ifdef LIB_EXPORTS
#define KONTAMABIL_EXPORTS __declspec(dllexport)
#else
#define KONTAMABIL_EXPORTS __declspec(dllimport)
#endif
#else
#define KONTAMABIL_EXPORTS
#endif

namespace PropertyEditor
{

/**
	@author BogDan Vatra <bogdan@licentia.eu>
*/
class PropertyInterface;
class PropertyModel;
class PropertyDelegate;
class KONTAMABIL_EXPORTS PropertyEditor : public QTreeView
{
	Q_OBJECT

public:
	PropertyEditor(QWidget *parent = 0);
	~PropertyEditor();

	QObject *object() const;

private:
	void loadPlugins();

public slots:
	void setObject(QObject * object);

private:
	QObject *m_object;
	QList<PropertyInterface*> m_plugins;
	PropertyModel* m_model;
	PropertyDelegate* m_delegate;
};

}

#endif
