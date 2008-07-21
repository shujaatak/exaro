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

#ifndef DESIGNERUIWIDGET_H
#define DESIGNERUIWIDGET_H

#include <QWidget>
#include "ui_designeruiwidget.h"

namespace Report
{

class DesignerUiWidget : public QWidget, private Ui::designerUiWidget
{
	Q_OBJECT
public:
	DesignerUiWidget(QWidget* parent = 0, Qt::WFlags fl = 0);
	~DesignerUiWidget();

	QMap <QString, QVariant> uis();
	void setUis(QMap <QString, QVariant> ius);

public slots:
	void createItem();
	void deleteItem();
	void editItem();
	void editName();

private:
	QMap <QString, QVariant> m_uis;

protected:
	void refreshButtons();

};

}
#endif

