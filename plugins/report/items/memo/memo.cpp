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

Memo::Memo(QGraphicsItem* parent, QObject* parentObject) : ItemInterfaceExt(parent, parentObject), m_textFlags(0), m_text(tr("Memo item")),m_sizePolicy(None)
{
	initMyResource();
	setWidth(25/UNIT);
	setHeight(5/UNIT);
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

QRectF Memo::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

bool Memo::prInit(Report::PaintInterface * paintInterface)
{
//    ItemInterface::init(paintInterface);
    storeAgregateValuesFromString(m_text);  // lookup for agragate functions
}

/*
void Memo::prePaint(QPainter * painter)
{
	ItemInterface::prePaint(painter);
	if (m_sizePolicy==None)
		return;

	QRectF rect = boundingRect();
	adjustRect(rect);
	QFontMetricsF fm(painter->font());
	m_printText = processString(m_text);
	if (m_sizePolicy==AutoSize)
	{
		qreal wd=fm.width(m_text);
		if (wd>width())
			setWidth(wd);
	}
	else
	{
		QRectF rc=fm.boundingRect(rect, TextFlags(), m_text);
		if (rc.height()>rect.height())
			setStretch(rc.height()-rect.height());
	}
}
*/
void Memo::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
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
	if (option->type == QStyleOption::SO_GraphicsItem)
	    painter->drawText(rect, textFlags(), m_text);
	else
	    if (scriptEngine())
		painter->drawText(rect, textFlags(), processString(m_text));
	painter->restore();
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit afterPrint(this);
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
