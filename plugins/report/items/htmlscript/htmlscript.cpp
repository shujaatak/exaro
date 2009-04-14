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
#include <QDebug>

#if QT_VERSION >= 0x040402
	#include <QWebPage>
	#include <QWebFrame>
#else
	#include <QTextDocument>
#endif

#include "htmlscript.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(htmlscript);
}

HtmlScript::HtmlScript(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject), m_script(tr("1+1")),m_sizeFlags(0)
{
	initMyResource();
	setWidth(25/UNIT);
	setHeight(5/UNIT);
}

QString HtmlScript::script()
{
	return m_script;
}

void HtmlScript::setHtmlScript(const QString & script)
{
	m_script = script;
	update();
}

QRectF HtmlScript::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

HtmlScript::SizeFlags HtmlScript::sizeFlags()
{
	return m_sizeFlags;
}
void HtmlScript::setSizeFlags(SizeFlags sizeFlags)
{
	m_sizeFlags=sizeFlags;
}

void HtmlScript::prepare(QPainter * painter)
{
	ItemInterface::prepare(painter);
	if (!m_sizeFlags)
		return;

#if QT_VERSION >= 0x040402
	QRectF rect = boundingRect();
	adjustRect(rect);
	QWebPage wp;
	wp.setViewportSize(QSize(rect.width()/2.54, rect.height()/2.54));
	wp.mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	wp.mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	wp.mainFrame()->setHtml(scriptEngine()->evaluate(m_script).toString());
	QSize pageSize=wp.mainFrame()->contentsSize();
	if (m_sizeFlags&AutoSizeHorizontally && pageSize.width()*2.54>rect.width())
		setWidth(pageSize.width()*2.54);
	if (m_sizeFlags&AutoSizeVertically && pageSize.height()*2.54>rect.height())
		setStretch(pageSize.height()*2.54);
#endif
}

void HtmlScript::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);
	adjustRect(rect);
	if (option->type == QStyleOption::SO_GraphicsItem)
		painter->drawText(rect, m_script);
	else
		if (scriptEngine())
		{
#if QT_VERSION >= 0x040402
			QWebPage wp;
			wp.setViewportSize(QSize(rect.width()/2.54, rect.height()/2.54));
			wp.mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
			wp.mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
			wp.mainFrame()->setHtml(scriptEngine()->evaluate(m_script).toString());
			painter->setClipRect( rect );
			painter->translate( rect.topLeft() );
			painter->scale( 2.54 ,  2.54 );
			//QRect r (0,0,rect.width()*2.54, rect.height()*2.54);
			wp.mainFrame()->render(painter);
#else
			QTextDocument td; // until QWebPage will work I think QTextDocument is the best choise
			td.setHtml(scriptEngine()->evaluate(m_script).toString());
			painter->setClipRect( rect );
			painter->translate(rect.topLeft());
			painter->scale( 2.54 ,  2.54 );
			td.drawContents(painter,QRectF(QPointF(0,0),rect.size()));
#endif
		}
}

QIcon HtmlScript::toolBoxIcon()
{
	return QIcon(":/htmlscript.png");
};

QString HtmlScript::toolBoxText()
{
	return tr("HtmlScript");
}

QString HtmlScript::toolBoxGroup()
{
	return tr("Text");
}

QObject * HtmlScript::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new HtmlScript(parent, parentObject);
}

Q_EXPORT_PLUGIN2(htmlScript, HtmlScript)
