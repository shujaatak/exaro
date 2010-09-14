/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2009 by BogDan Vatra                                    *
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

#ifndef EXAROTOOLBARSINTERFACE_H
#define EXAROTOOLBARSINTERFACE_H

#include <QToolBar>

namespace eXaro
{

class ToolBarInterface : public QToolBar
{
	Q_OBJECT
public:
	ToolBarInterface(const QString & title="", QWidget * parent = 0 );
	~ToolBarInterface();

public slots:
	virtual void setObject(QObject * object) = 0;
	virtual void propertyChanged(QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value) = 0;

signals:
	void propertyChanged();

};

}

Q_DECLARE_INTERFACE(eXaro::ToolBarInterface, "ro.bigendian.eXaro.ToolBarInterface/1.0");

#endif
