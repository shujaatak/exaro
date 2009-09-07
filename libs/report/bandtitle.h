/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 *   Copyright (C) 2009 by Mikhalov Alexaner                               *
 *   alexmi3@rambler.ru                                                    *
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

#ifndef BANDTITLE_H
#define BANDTITLE_H

#include <QGraphicsItem>
#include <QString>
//#include <bandinterface.h>

namespace Report
{

class BandInterface;

class BandTitle : public QGraphicsItem
{
    public:
	BandTitle( BandInterface * parentBand, const QSizeF & size, const QString & text, QGraphicsItem * parent=0 );

	void setSize(QSizeF size);
	inline const QSizeF & size(){return m_size;}
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void setCaption(QString str);
	QString caption();
    protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

    private:
	QSizeF m_size;
	QString m_text;
	BandInterface * m_parentBand;
};

}

#endif // BANDTITLE_H
