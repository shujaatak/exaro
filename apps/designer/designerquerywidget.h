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


#ifndef DESIGNERQUERYWIDGET_H
#define DESIGNERQUERYWIDGET_H

#include <QWidget>
#include "ui_designerquerywidget.h"


namespace Report
{

class DesignerQueryWidget : public QWidget, public Ui::designerQueryWidget
{
	Q_OBJECT

private:
	QMap <QString, QVariant> m_queries;

protected:
	void refreshButtons();

public:
	DesignerQueryWidget(QWidget* parent = 0, Qt::WFlags fl = 0);
	~DesignerQueryWidget();

	QMap <QString, QVariant> queries();
	void setQueries(QMap <QString, QVariant> queries);


public slots:
	void createItem();
	void deleteItem();
	void editItem();
	void editName();
};

}

#endif

