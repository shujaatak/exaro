/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by Mikhalov Alexander (aka AlFoX)                  *
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

#ifndef PICTURE_H
#define PICTURE_H

#include <iteminterface.h>


class Picture : public Report::ItemInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);

	Q_ENUMS(PictureTypes PaintTypes FrameTypes DrawTextTypes);
	Q_PROPERTY(PaintTypes howToPaintPicture READ paintType WRITE setPaintType)
	Q_PROPERTY(DrawTextTypes drawTextType READ drawTextType WRITE setDrawTextType)
	Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
	Q_PROPERTY(QString text READ comment WRITE setComment)
	Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
	Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
	Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)

public:
	enum DrawTextTypes
	{
		NO_TEXT = 0,
		DRAW_TOP = 1,
		DRAW_BOTTOM = 2,
		DRAW_ABOVE = 3,
		DRAW_BELOW = 4
	};
	enum FrameTypes
	{
		NO_FRAME = 0,
		PICTURE_BIND = 1,
		PICTURE_BOX = 2
	};

	enum PaintTypes
	{
		RESIZE_ITEM_TO_PICTURE = 1,
		CENTER_PICTURE_TO_ITEM = 2,
		RESIZE_PICTURE_TO_ITEM_KEEP_ASPECT_RATIO = 3,
		RESIZE_PICTURE_TO_ITEM_IGNORE_ASPECT_RATIO = 4
	};


public:
	Picture(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();


	PaintTypes paintType();
	void setPaintType(PaintTypes paintType);

	FrameTypes frameType();
	void setFrameType(FrameTypes frameType);

	DrawTextTypes drawTextType();
	void setDrawTextType(DrawTextTypes drawTextType);

	QPixmap pixmap();
	void setPixmap(const QPixmap & pixmap);

	QString comment();
	void setComment(const QString & comment);

	QColor textColor();
	void setTextColor(const QColor & color);

	int borderWidth();
	void setBorderWidth(int bWidth);

	QColor borderColor();
	void setBorderColor(const QColor & color);

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

private:
	PaintTypes m_paintType;
	FrameTypes m_frameType;
	DrawTextTypes m_drawTextType;
	QString m_comment;
	QString m_primaryMessage;
	QString m_script;
	QImage  m_image;
	QColor m_textColor;
	QColor m_borderColor;
	int m_borderWidth;
};


#endif
