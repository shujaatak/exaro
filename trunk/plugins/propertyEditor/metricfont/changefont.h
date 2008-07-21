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

#ifndef CHANGEFONT_H
#define CHANGEFONT_H

#include <QPushButton>

/**
	@author BogDan Vatra <bogdan@licentia.eu>
*/
class ChangeFont : public QPushButton
{
	Q_OBJECT
public:
	ChangeFont(QWidget *parent = 0);

	void setFont(QFont font);
	QFont font();

public slots:
	void onClick();

signals:
	void fontChanged(const QFont & font);

private:
	QFont m_font;
};

#endif
