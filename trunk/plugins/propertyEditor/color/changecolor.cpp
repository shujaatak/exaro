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

#include <QColorDialog>

#include "changecolor.h"

ChangeColor::ChangeColor(QWidget *parent)
		: QPushButton(parent)
{
	connect(this, SIGNAL(pressed()), this, SLOT(onClick()));
	setText(tr("Change color"));
}


void ChangeColor::onClick()
{
	QColor color = QColorDialog::getColor(m_color, this);
	if (color.isValid())
	{
		m_color = color;
		emit(colorChanged(m_color));
	}
}

void ChangeColor::setColor(QColor color)
{
	m_color = color;
}

QColor ChangeColor::color()
{
	return m_color;
}
