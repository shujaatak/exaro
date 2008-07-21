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

#include "field.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(field);
}

Field::Field(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject), m_textFlags(0),m_fieldName(tr("field")),m_queryName(tr("query"))
{
	initMyResource();
	setWidth(30/UNIT);
	setHeight(5/UNIT);
}

Field::TextFlags Field::textFlags()
{
	return m_textFlags;
}

void Field::setTextFlags(TextFlags textFlags)
{
	m_textFlags = textFlags;
	update();
}

QString Field::fieldName()
{
	return m_fieldName;
}

void Field::setFieldName(const QString &fieldName)
{
	m_fieldName=fieldName;
	update();
}

QString Field::queryName()
{
	return m_queryName;
}

void Field::setQueryName(const QString &queryName)
{
	m_queryName=queryName;
	update();
}


QRectF Field::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Field::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	if (option->type != QStyleOption::SO_GraphicsItem)
		emit beforePrint(this);

	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());
	setupPainter(painter);

	adjustRect(rect);

	painter->drawText(rect, textFlags(), option->type == QStyleOption::SO_GraphicsItem?(m_queryName+'.'+m_fieldName):queryField(m_queryName,m_fieldName).toString());

	if (option->type != QStyleOption::SO_GraphicsItem)
		emit afterPrint(this);
}

QIcon Field::toolBoxIcon()
{
	return QIcon(":/field.png");
};

QString Field::toolBoxText()
{
	return tr("Field");
}

QString Field::toolBoxGroup()
{
	return tr("Text");
}

QObject * Field::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Field(parent, parentObject);
}

Q_EXPORT_PLUGIN2(text, Field)
