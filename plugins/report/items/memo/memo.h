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

#ifndef MEMO_H
#define MEMO_H

#include <iteminterface.h>
//#include <itemhelper.h>

namespace Report
{
    class ItemHelper;
}

class Memo : public Report::ItemInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);

	Q_FLAGS(TextFlags);
	Q_ENUMS(SizePolicy);
	Q_PROPERTY(TextFlags textFlags READ textFlags WRITE setTextFlags)
	Q_PROPERTY(SizePolicy sizePolicy READ sizePolicy WRITE setSizePolicy)
	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
	Q_PROPERTY(QFont font READ font WRITE setFont)

public:

	enum TextFlag 
	{
		AlignLeft = Qt::AlignLeft,
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

	Q_DECLARE_FLAGS(TextFlags, TextFlag);

public:
	Memo(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	/**
	 * Return the item font
	 * @return item font
	 * @see setFont()
	 */
	virtual QFont font();
	/**
	 * Set the item font
	 * @param font item font
	 * @see font()
	 */
	virtual void setFont(const QFont & font);
	virtual Report::ItemHelper * createHelper();

	QRectF boundingRect() const;

	bool prInit(Report::PaintInterface * paintInterface);

	void _paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QRectF & rect, QWidget * widget = 0);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	SizePolicy sizePolicy();
	void setSizePolicy(SizePolicy sizePolicy);

	TextFlags textFlags();
	void setTextFlags(TextFlags textFlags);

	QString text();
	void setText(const QString &text);

	QColor textColor();
	void setTextColor(const QColor & color);

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

private:
	TextFlags m_textFlags;
	QString m_text, m_printText;
	SizePolicy m_sizePolicy;
	QFont m_font;
	QColor m_textColor;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Memo::TextFlags);

#endif
