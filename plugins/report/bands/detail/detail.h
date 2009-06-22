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

#ifndef DETAIL_H
#define DETAIL_H

#include <bandinterface.h>

class Detail : public Report::BandInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);
	Q_PROPERTY(QString dataset READ dataset WRITE setDataset);
	Q_PROPERTY(bool zebra READ isZebra WRITE setZebra);
	Q_PROPERTY(QString joinTo READ joinTo WRITE setJoinTo);

public:
	Detail(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	~Detail();

	int layoutPriority(){ return 50;}

	bool prInit(Report::PaintInterface * paintInterface);
	bool prData();
	bool prReset();

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	bool canContain(QObject * object);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	bool isZebra();			    //light and dark rows
	void setZebra(bool b);

	QString joinTo();
	void setJoinTo(QString bandName);

public slots:
	void joinToSlot(QObject * item);
private:
	bool m_isZebra;
	bool odd;	//counter for zebra
	QString m_joinTo;

};

#endif
