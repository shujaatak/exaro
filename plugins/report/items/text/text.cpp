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

#include "text.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(text);
}

Text::Text(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject), m_textFlags(0), m_text(tr("text item")),m_sizePolicy(None)
{
	initMyResource();
	setWidth(25/UNIT);
	setHeight(5/UNIT);
}

Text::SizePolicy Text::sizePolicy()
{
	return m_sizePolicy;
}
void Text::setSizePolicy(SizePolicy sizePolicy)
{
	m_sizePolicy=sizePolicy;
}

Text::TextFlags Text::textFlags()
{
	return m_textFlags;
}

void Text::setTextFlags(TextFlags textFlags)
{
	m_textFlags = textFlags;
	update();
}

QString Text::text()
{
	return m_text;
}

void Text::setText(const QString &text)
{
	m_text = text;
	update();
}

QRectF Text::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Text::prepare(QPainter * painter)
{
	ItemInterface::prepare(painter);
	if (m_sizePolicy==None)
		return;

	QRectF rect = boundingRect();
	adjustRect(rect);
	QFontMetricsF fm(painter->font());
	if (m_sizePolicy==AutoSize)
	{
		qreal wd=fm.width(m_text);
		if (wd>width())
			setWidth(wd);
	}
	else
	{
		QRectF rc=fm.boundingRect(rect, textFlags(), m_text);
		if (rc.height()>rect.height())
			setStretch(rc.height()-rect.height());
	}
}

void Text::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);
	adjustRect(rect);

	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->drawText(rect, textFlags(), m_text);
}

QIcon Text::toolBoxIcon()
{
	return QIcon(":/text.png");
};

QString Text::toolBoxText()
{
	return tr("Text");
}

QString Text::toolBoxGroup()
{
	return tr("Text");
}

QObject * Text::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Text(parent, parentObject);
}

Q_EXPORT_PLUGIN2(text, Text)
