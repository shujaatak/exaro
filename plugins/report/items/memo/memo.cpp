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

#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "memo.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(memo);
}

Memo::Memo(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject), m_textFlags(0), m_text(tr("Memo item")),m_sizePolicy(None)
{
	initMyResource();
	setWidth(25/UNIT);
	setHeight(5/UNIT);
	m_font=QFont("Serif");
	m_font.setPointSizeF(3.5);
	m_font.setStyleStrategy(QFont::PreferMatch);
	m_font.setStyleStrategy(QFont::ForceOutline);
}

Memo::SizePolicy Memo::sizePolicy()
{
	return m_sizePolicy;
}
void Memo::setSizePolicy(SizePolicy sizePolicy)
{
	m_sizePolicy=sizePolicy;
}

Memo::TextFlags Memo::textFlags()
{
	return m_textFlags;
}

void Memo::setTextFlags(TextFlags textFlags)
{
	m_textFlags = textFlags;
	update();
}

QString Memo::text()
{
	return m_text;
}

void Memo::setText(const QString &text)
{
	m_text = text;
	update();
}

QFont Memo::font()
{
	return m_font;
}

void Memo::setFont(const QFont & font)
{
	m_font = font;
	update();
}

QColor Memo::textColor()
{
    return m_textColor;
}

void Memo::setTextColor(const QColor & color)
{
    m_textColor = color;
    update();

}

QRectF Memo::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

bool Memo::prInit(Report::PaintInterface * paintInterface)
{
    storeAgregateValuesFromString(m_text);  // lookup for agragate functions
}

void Memo::_paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QRectF & rect,  QWidget * /*widget*/)
{
	QFont f=font();
	f.setPixelSize(font().pointSizeF()/UNIT);
	painter->setFont(f);

	painter->setPen(m_textColor);
	if (option->type == QStyleOption::SO_GraphicsItem)
	    painter->drawText(rect, textFlags(), m_text);
	else
	    if (scriptEngine())
		painter->drawText(rect, textFlags(), processString(m_text));
}

QIcon Memo::toolBoxIcon()
{
	return QIcon(":/memo.png");
};

QString Memo::toolBoxText()
{
	return tr("Memo");
}

QString Memo::toolBoxGroup()
{
	return tr("Memo");
}

QObject * Memo::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Memo(parent, parentObject);
}

Q_EXPORT_PLUGIN2(memo, Memo)
