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
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iteminterface.h>

class Rectangle : public Report::ItemInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);
	Q_PROPERTY(qreal xRadius READ xRadius WRITE setXRadius)
	Q_PROPERTY(qreal yRadius READ yRadius WRITE setYRadius)

public:
	Rectangle(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	bool canContain(QObject * object);

	qreal xRadius();
	void setXRadius(qreal radius);

	qreal yRadius();
	void setYRadius(qreal radius);

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);
private:
	qreal m_xRadius, m_yRadius;
};

#endif
