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

#ifndef SCRIPT_H
#define SCRIPT_H

#include <iteminterface.h>

class HtmlScript : public Report::ItemInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);
	Q_FLAGS(SizeFlags);

	Q_PROPERTY(SizeFlags sizeFlags READ sizeFlags WRITE setSizeFlags)
	Q_PROPERTY(QString script READ script WRITE setHtmlScript)
public:
	enum SizeFlag
	{
		AutoSizeHorizontally = 1,
		AutoSizeVertically = 2
	};
	Q_DECLARE_FLAGS(SizeFlags, SizeFlag);

public:
	HtmlScript(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	QRectF boundingRect() const;
	void prepare(QPainter * painter);
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	SizeFlags sizeFlags();
	void setSizeFlags(SizeFlags sizeFlags);

	QString script();
	void setHtmlScript(const QString &text);

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

private:
	QString m_script;
	SizeFlags m_sizeFlags;
};

#endif
