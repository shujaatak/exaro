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

#ifndef PROPERTYEDITORPROPERTYDELEGATE_H
#define PROPERTYEDITORPROPERTYDELEGATE_H

#include <QItemDelegate>


namespace PropertyEditor
{

/**
	@author BogDan Vatra <bogdan@licentia.eu>
*/
class PropertyDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	PropertyDelegate(QObject *parent = 0);

	~PropertyDelegate();

	QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;

	QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

	void setEditorData(QWidget * editor, const QModelIndex & index) const;

	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

}

#endif
