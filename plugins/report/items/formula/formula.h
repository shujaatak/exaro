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

#ifndef FORMULA_H
#define FORMULA_H

#include <iteminterface.h>

class Formula : public Report::ItemInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);

	Q_FLAGS(TextFlags);
	Q_ENUMS(SizePolicy FormulaInput FormulaType);
	Q_PROPERTY(TextFlags textFlags READ textFlags WRITE setTextFlags)
	Q_PROPERTY(FormulaInput formulaInput READ formulaInput WRITE setFormulaInput)
	Q_PROPERTY(FormulaType formulaType READ formulaType WRITE setFormulaType)
	Q_PROPERTY(QString queryName READ queryName WRITE setQueryName)
	Q_PROPERTY(QString fieldName READ fieldName WRITE setFieldName)
	Q_PROPERTY(QString script READ script WRITE setScript)
	Q_PROPERTY(int precision READ precision WRITE setPrecision)
	Q_PROPERTY(bool resetAfterPrint READ resetAfterPrint WRITE setResetAfterPrint)

public:
	enum TextFlag {AlignLeft = Qt::AlignLeft,
	               AlignRight = Qt::AlignRight,
	               AlignHCenter = Qt::AlignHCenter,
	               AlignJustify = Qt::AlignJustify,
	               AlignTop = Qt::AlignTop,
	               AlignBottom = Qt::AlignBottom,
	               AlignVCenter = Qt::AlignVCenter,
	               AlignCenter = Qt::AlignCenter,
	               TextDontClip = Qt::TextDontClip,
	               TextSingleLine = Qt::TextSingleLine,
	               TextExpandTabs = Qt::TextExpandTabs,
	               TextShowMnemonic = Qt::TextShowMnemonic,
	               TextWordWrap = Qt::TextWordWrap
	              };

	enum SizePolicy
	{
		None = 0,
		AutoSize = 1,
		AutoStretch = 2
	};

	enum FormulaInput
	{
		FromField = 0,
		FromScript = 1
	};

	enum FormulaType
	{
		Sum = 1,
		Count = 2,
		Max = 3,
		Min = 4,
		Average = 5
	};

	Q_DECLARE_FLAGS(TextFlags, TextFlag);

public:
	Formula(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	QRectF boundingRect() const;

	void prepare(QPainter * painter);
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	SizePolicy sizePolicy();
	void setSizePolicy(SizePolicy sizePolicy);

	TextFlags textFlags();
	void setTextFlags(TextFlags textFlags);

	FormulaInput formulaInput();
	void setFormulaInput(FormulaInput formulaInput);

	FormulaType formulaType();
	void setFormulaType(FormulaType formulaType);

	QString queryName();
	void setQueryName(const QString &text);

	QString fieldName();
	void setFieldName(const QString &text);

	QString script();
	void setScript(QString script);

	bool resetAfterPrint();
	void setResetAfterPrint(bool resetAfterPrint);

	int precision();
	void setPrecision(int precision);

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

private:
	qreal getValue();
	QString computeValue();
	QString formatDesignerString();

private slots:
	void afterNext();
	void afterPrev();
	void connectToQuery();

private:
	TextFlags m_textFlags;
	QString m_fieldName,m_queryName;
	SizePolicy m_sizePolicy;
	FormulaType m_formulaType;
	FormulaInput m_formulaInput;
	QString m_script;
	bool m_resetAfterPrint;
	int m_precision;
	QList<qreal> m_values;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Formula::TextFlags);

#endif
