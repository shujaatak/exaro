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
#include <QIcon>

#include "fonttoolbar.h"

inline void initMyResource()
{
	Q_INIT_RESOURCE(fonttoolbar);
}

FontToolbar::FontToolbar(const QString & title, QWidget * parent):QToolBar(title, parent)
{
	initMyResource();
	setWindowIcon(QIcon(":/fonttoolbar.png"));
	setWindowTitle(tr("Font toolbar"));
	setObjectName("fontToolbar");
	m_fontFamilyComboBox = new QFontComboBox(this);
	m_fontFamilyComboBox->setFontFilters(QFontComboBox::AllFonts);
	connect(m_fontFamilyComboBox,SIGNAL(currentFontChanged(QFont)), SLOT(setFontFamily(QFont)));
	addWidget(m_fontFamilyComboBox);

	m_fontStyleComboBox = new QComboBox(this);
	m_fontStyleComboBox->setModel(&m_fontStyleModel);
	connect(m_fontStyleComboBox, SIGNAL(activated(QString)), SLOT(setFontStyle(QString)));
	addWidget(m_fontStyleComboBox);

	m_fontSizeComboBox = new QComboBox(this);
	m_fontSizeComboBox->setEditable(true);
	m_fontSizeComboBox->setModel(&m_fontSizeModel);
	connect(m_fontSizeComboBox, SIGNAL(activated(QString)), SLOT(setFontSize(QString)));
	addWidget(m_fontSizeComboBox);
	m_fontSizeModel.setStringList(QStringList()<<"2.5"<<"3.0"<<"3.5"<<"4.0"<<"4.5"<<"5.0"<<"6.0"<<"7.0"<<"8.0"<<"9.0"<<"10.0");
	setEnabled(false);
}

void FontToolbar::setFontFamily(const QFont & font)
{
	if (m_object)
	{
		QFont f=m_object->property("font").value<QFont>();
		f.setFamily(font.family());
		m_object->setProperty("font", f);
		emit(propertyChanged());
	}
}

void FontToolbar::setFontStyle(const QString & style)
{
	if (m_object)
	{
		QFont f=m_object->property("font").value<QFont>();
		QFont ft=m_fontDatabase.font(f.family(), style ,10);
		f.setStyle(ft.style());
		f.setBold(ft.bold());
		f.setItalic(ft.italic());
		m_object->setProperty("font", f);
		emit(propertyChanged());
	}
}

void FontToolbar::setFontSize(const QString & size)
{
	if (m_object)
	{
		QFont f=m_object->property("font").value<QFont>();
		bool ok=false;
		f.setPointSizeF(size.toDouble(&ok));
		if (ok)
		{
			m_object->setProperty("font", f);
			emit(propertyChanged());
		}
	}
}

void FontToolbar::propertyChanged(QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value)
{
	if(propertyName=="font")
		setObject(obj);
}

void FontToolbar::setObject(QObject* object) 
{
	m_object=object;
	setEnabled(false);
	if (!m_object)
		return;
	QVariant font=m_object->property("font");
	if (font.isValid())
	{
		m_fontFamilyComboBox->setCurrentFont(font.value<QFont>());
		m_fontStyleModel.setStringList(m_fontDatabase.styles(font.value<QFont>().family()));
		m_fontStyleComboBox->setCurrentIndex(m_fontStyleModel.index(m_fontDatabase.styles(font.value<QFont>().family()).indexOf(m_fontDatabase.styleString(font.value<QFont>())),0).row());
		m_fontSizeComboBox->setEditText(QString::number(font.value<QFont>().pointSizeF()));
		setEnabled(true);
	}
}

Q_EXPORT_PLUGIN2(fontToolbar, FontToolbar)
