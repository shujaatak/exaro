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

#include <QSqlDatabase>
#include <QStringListModel>
#include <QMessageBox>
#include <QSqlError>
#include <QFileDialog>
#include <QDir>

#include "sqldatabasedialog.h"

SqlDatabaseDialog::SqlDatabaseDialog(QWidget* parent, Qt::WFlags fl)
		: QDialog(parent, fl), Ui::databaseDialog()
{
	setupUi(this);
	openButton->setIcon(QIcon(":/images/openReport.png"));
	drivers->setModel(new QStringListModel(QSqlDatabase::drivers(), this));
}

SqlDatabaseDialog::~SqlDatabaseDialog()
{
}

void SqlDatabaseDialog::accept()
{
	QSqlDatabase db = QSqlDatabase::addDatabase(drivers->currentText());
	db.setDatabaseName(database->text());
	db.setHostName(host->text());
	db.setPort(port->text().toInt());
	db.setUserName(user->text());
	db.setPassword(password->text());
	if (!db.open())
		QMessageBox::critical(this, tr("Connection error"), db.lastError().text(), QMessageBox::Ok);
	else
		QDialog::accept();
}

void SqlDatabaseDialog::on_openButton_clicked()
{
	QString file=QFileDialog::getOpenFileName(this,  tr("Open SQLite Database"), QDir::homePath(), tr("SQLite Database (*.db *.db3 *.sqlite *.sqlite3);;All files (*.*)"));
	if (file.length())
		database->setText(file);
}

void SqlDatabaseDialog::on_drivers_currentIndexChanged ( int /*index*/ )
{
	openButton->setEnabled(drivers->currentText().contains("QSQLITE"));
}
