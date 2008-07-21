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

#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "text.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(text);
}

Text::Text(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject), m_textFlags(0), m_text(tr("text item"))
{
	initMyResource();
	setWidth(25/UNIT);
	setHeight(5/UNIT);
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

void Text::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit beforePrint(this);

	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());
	setupPainter(painter);

	adjustRect(rect);

	painter->save();
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->drawText(rect, textFlags(), m_text);
	painter->restore();

	if (option->type != QStyleOption::SO_GraphicsItem)
		emit afterPrint(this);
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
