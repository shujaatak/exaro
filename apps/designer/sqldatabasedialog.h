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

#ifndef SQLDATABASEDIALOG_H
#define SQLDATABASEDIALOG_H

#include <QDialog>
#include "ui_sqldatabasedialog.h"

class SqlDatabaseDialog : public QDialog, private Ui::databaseDialog
{
	Q_OBJECT

public:
	SqlDatabaseDialog(QWidget* parent = 0, Qt::WFlags fl = 0);
	~SqlDatabaseDialog();

protected slots:
	virtual void	accept();
	void on_openButton_clicked();
	void on_drivers_currentIndexChanged(int index);
	void on_pbSave_clicked();
	void on_cbConnections_activated(QString text);
private:
	QMap<QString, QVariant> m_connList;
};

#endif

