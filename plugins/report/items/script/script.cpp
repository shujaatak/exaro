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

#include "script.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(script);
}

Script::Script(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject), m_textFlags(0), m_script(tr("1+1")),m_sizePolicy(None)
{
	initMyResource();
	setWidth(25/UNIT);
	setHeight(5/UNIT);
}

Script::SizePolicy Script::sizePolicy()
{
	return m_sizePolicy;
}
void Script::setSizePolicy(SizePolicy sizePolicy)
{
	m_sizePolicy=sizePolicy;
}

Script::TextFlags Script::textFlags()
{
	return m_textFlags;
}

void Script::setTextFlags(TextFlags textFlags)
{
	m_textFlags = textFlags;
	update();
}

QString Script::script()
{
	return m_script;
}

void Script::setScript(const QString & script)
{
	m_script = script;
	update();
}

QRectF Script::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Script::prepare(QPainter * painter)
{
	ItemInterface::prepare(painter);
	if (m_sizePolicy==None)
		return;

	QRectF rect = boundingRect();
	adjustRect(rect);
	QFontMetricsF fm(painter->font());
	QString m_text=scriptEngine()->evaluate(m_script).toString();
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

void Script::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);
	adjustRect(rect);

	if (option->type == QStyleOption::SO_GraphicsItem)
		painter->drawText(rect, textFlags(), m_script);
	else
		if (scriptEngine())
			painter->drawText(rect, textFlags(), scriptEngine()->evaluate(m_script).toString());
}

QIcon Script::toolBoxIcon()
{
	return QIcon(":/script.png");
};

QString Script::toolBoxText()
{
	return tr("Script");
}

QString Script::toolBoxGroup()
{
	return tr("Text");
}

QObject * Script::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Script(parent, parentObject);
}

Q_EXPORT_PLUGIN2(script, Script)
