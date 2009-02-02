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

#ifndef DETAILHEADER_H
#define DETAILHEADER_H

#include <bandinterface.h>

class DetailHeader : public Report::BandInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);
	Q_PROPERTY(QString groupField READ groupField WRITE setGroupField)
	Q_PROPERTY(bool reprintOnNewPage READ reprintOnNewPage WRITE setReprintOnNewPage)
	Q_PROPERTY(bool resetDetailNumber READ resetDetailNumber WRITE setResetDetailNumber)
	Q_PROPERTY(bool forceNewPage READ forceNewPage WRITE setForceNewPage)
public:
	DetailHeader(QGraphicsItem* parent = 0, QObject* parentObject = 0);
	~DetailHeader();

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	bool canContain(QObject * object);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

};

#endif
