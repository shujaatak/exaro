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

#ifndef FLAGSLIST_H
#define FLAGSLIST_H

#include <QDialog>
#include "ui_flagsList.h"

class FlagsList : public QDialog, public Ui::Dialog
{
	Q_OBJECT

public:
	FlagsList(QWidget* parent = 0, Qt::WFlags fl = 0);
	~FlagsList();

protected slots:
	void checkAll();
	void uncheckAll();

};

#endif

