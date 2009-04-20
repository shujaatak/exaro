/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2009 by BogDan Vatra                                    *
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
#include <sqlquery.h>
#include <reportinterface.h>

#include "formula.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(formula);
}

Formula::Formula(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject), 
		m_textFlags(0), 
		m_fieldName(tr("field")),
		m_queryName(tr("query")),
		m_sizePolicy(None), 
		m_formulaType(Sum), 
		m_formulaInput(FromField),
		m_resetAfterPrint(true),
		m_precision(6)
{
	initMyResource();
	setWidth(30/UNIT);
	setHeight(5/UNIT);
	Report::ReportInterface	* ro=reportObject();
	if (ro)
		connect(ro,SIGNAL(beforeExec()), SLOT(connectToQuery()));
}

Formula::SizePolicy Formula::sizePolicy()
{
	return m_sizePolicy;
}
void Formula::setSizePolicy(SizePolicy sizePolicy)
{
	m_sizePolicy=sizePolicy;
}

Formula::TextFlags Formula::textFlags()
{
	return m_textFlags;
}

void Formula::setTextFlags(TextFlags textFlags)
{
	m_textFlags = textFlags;
	update();
}

Formula::FormulaInput Formula::formulaInput() 
{
	return m_formulaInput;
}

void Formula::setFormulaInput(Formula::FormulaInput formulaInput) 
{
	m_formulaInput=formulaInput;
	update();
}


Formula::FormulaType Formula::formulaType() 
{
	return m_formulaType;
}

void Formula::setFormulaType(Formula::FormulaType formulaType) 
{
	m_formulaType=formulaType;
	update();
}

QString Formula::queryName()
{
	return m_queryName;
}

void Formula::setQueryName(const QString &queryName)
{
	m_queryName=queryName;
	update();
}

QString Formula::script() 
{
	return m_script;
}

void Formula::setScript(QString script) 
{
	m_script=script;
	update();
}

bool Formula::resetAfterPrint() 
{
	return m_resetAfterPrint;
}

void Formula::setResetAfterPrint(bool resetAfterPrint) 
{
	m_resetAfterPrint=resetAfterPrint;
}

QString Formula::fieldName()
{
	return m_fieldName;
}

void Formula::setFieldName(const QString &fieldName)
{
	m_fieldName=fieldName;
	update();
}

int Formula::precision()
{
	return m_precision;
}

void Formula::setPrecision(int precision)
{
	m_precision=precision;
}


QRectF Formula::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Formula::prepare(QPainter * painter)
{
	ItemInterface::prepare(painter);
	if (m_sizePolicy==None)
		return;

	QRectF rect = boundingRect();
	adjustRect(rect);
	QFontMetricsF fm(painter->font());
	QString m_text=computeValue();
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

void Formula::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);
	adjustRect(rect);
	if (option->type == QStyleOption::SO_GraphicsItem)
		painter->drawText(rect, textFlags(), formatDesignerString());
	else
	{
		painter->drawText(rect, textFlags(), computeValue());
		if (m_resetAfterPrint)
			m_values.clear();
	}
}

QIcon Formula::toolBoxIcon()
{
	return QIcon(":/formula.png");
};

QString Formula::toolBoxText()
{
	return tr("Formula");
}

QString Formula::toolBoxGroup()
{
	return tr("Text");
}

QObject * Formula::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Formula(parent, parentObject);
}

qreal Formula::getValue()
{
	if (m_formulaInput==FromField)
		return queryField(m_queryName,m_fieldName).toDouble();
	else
		return scriptEngine()->evaluate(m_script).toNumber();
}

QString Formula::computeValue() 
{
	qreal value=0;
	if (m_values.size())
		switch (m_formulaType)
		{
			case Sum:
				foreach(qreal v, m_values)
					value+=v;
				break;
			case Count:
				value=m_values.size();
				break;
			case Max:
				value=m_values[0];
				foreach(qreal v, m_values)
					if (value<v)
						value=v;
				break;
			case Min:
				value=m_values[0];
				foreach(qreal v, m_values)
					if (value>v)
						value=v;
				break;
			case Average:
				foreach(qreal v, m_values)
					value+=v;
				value/=m_values.size();
				break;
		}
	return QString::number(value,'f', m_precision);
}

QString Formula::formatDesignerString()
{
	QString val=(m_formulaInput==FromField)?QString("%1.%2").arg(m_queryName).arg(m_fieldName):m_script;
	switch (m_formulaType)
	{
		case Sum:
			return QString("sum(%1)").arg(val);
		case Count:
			return QString("count(%1)").arg(val);
			break;
		case Max:
			return QString("max(%1)").arg(val);
		case Min:
			return QString("min(%1)").arg(val);
		case Average:
			return QString("average(%1)").arg(val);
	}
	return "";
}

void Formula::afterNext()
{
	m_values.push_back(getValue());
}

void Formula::afterPrev()
{
	m_values.pop_back();
}

void Formula::connectToQuery()
{
	Report::SqlQuery * qry=findQuery(m_queryName);
	if (qry)
	{
		connect(qry,SIGNAL(afterFirst()), SLOT(afterNext()));
		connect(qry,SIGNAL(afterNext()), SLOT(afterNext()));
		connect(qry,SIGNAL(afterPrevious()),SLOT(afterPrev()));
	}
}

Q_EXPORT_PLUGIN2(formula, Formula)
