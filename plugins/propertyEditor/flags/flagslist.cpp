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


#include "flagslist.h"

FlagsList::FlagsList(QWidget* parent, Qt::WFlags fl)
		: QDialog(parent, fl), Ui::Dialog()
{
	setupUi(this);
	connect(checkAllButton, SIGNAL(pressed()), this, SLOT(checkAll()));
	connect(uncheckAllButton, SIGNAL(pressed()), this, SLOT(uncheckAll()));
}

void FlagsList::checkAll()
{
	for (int fl = 0;fl < list->count();fl++)
		list->item(fl)->setCheckState(Qt::Checked);
}

void FlagsList::uncheckAll()
{
	for (int fl = 0;fl < list->count();fl++)
		list->item(fl)->setCheckState(Qt::Unchecked);
}

FlagsList::~FlagsList()
{
}


