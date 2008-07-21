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

#include "metricfontdialog.h"

#include "changefont.h"

ChangeFont::ChangeFont(QWidget *parent)
		: QPushButton(parent)
{
	connect(this, SIGNAL(pressed()), this, SLOT(onClick()));
	setText(tr("Change font"));
}

void ChangeFont::onClick()
{
	QFont font = MetricFontDialog::getFont(m_font, this, tr("Choose a font"));
	m_font = font;
	emit(fontChanged(m_font));
}

void ChangeFont::setFont(QFont font)
{
	m_font = font;
}

QFont ChangeFont::font()
{
	return m_font;
}
