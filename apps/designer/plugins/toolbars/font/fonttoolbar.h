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

#ifndef FONTTOOLBAR_H
#define FONTTOOLBAR_H

#include <QToolBar>
#include <QComboBox>
#include <QFontComboBox>
#include <QStringListModel>
#include <QItemSelectionModel>
#include <QFontDatabase>

class FontToolbar : public QToolBar
{
	Q_OBJECT

public:
	FontToolbar(const QString & title=0, QWidget * parent=0);

public slots:
	void setObject(QObject * object);
	void propertyChanged(QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value);

private slots:
	void setFontFamily(const QFont & font);
	void setFontStyle(const QString & style);
	void setFontSize(const QString & size);

signals:
	void propertyChanged();

private:
	QObject * m_object;
	QFontComboBox * m_fontFamilyComboBox;
	QComboBox * m_fontStyleComboBox;
	QComboBox * m_fontSizeComboBox;
	QFontDatabase m_fontDatabase;

	QStringListModel m_fontStyleModel;
	QStringListModel m_fontSizeModel;

};

#endif
