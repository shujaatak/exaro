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

#ifndef CHANGECOLOR_H
#define CHANGECOLOR_H

#include <QPushButton>

/**
	@author BogDan Vatra <bogdan@licentia.eu>
*/
class ChangeColor : public QPushButton
{
	Q_OBJECT
public:

	ChangeColor(QWidget *parent = 0);
	void setColor(QColor font);
	QColor color();

public slots:
	void onClick();

signals:
	void colorChanged(const QColor & color);

private:
	QColor m_color;
};

#endif
