/***************************************************************************
 *   Copyright (C) 2009 by BogDan Vatra                                    *
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

#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

#include "querywizardpage.h"
#include "queryeditdialog.h"
#include "sqldatabasedialog.h"
#include "sqlquery.h"

queryWizardPage::queryWizardPage(Report::ReportEngine* reportEngine, Report::ReportInterface* reportInterface, QWidget* parent):QWizardPage(parent), m_reportEngine(reportEngine), m_report(reportInterface)
{
	setupUi(this);
	setPixmap(QWizard::BackgroundPixmap,QPixmap(":/images/queryWizard.png"));
	removeQuery->setEnabled(false);
}

void queryWizardPage::on_setupDatabase_clicked()
{
	SqlDatabaseDialog d;
	d.exec();
}

void queryWizardPage::on_addQuery_clicked()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Create a query object"), tr(" query name:"), QLineEdit::Normal, QString("query_%1").arg(queriesList->count()), &ok);
	if (!ok || text.isEmpty())
		return;
	QueryEditDialog d;
	d.setQuery("SELECT * FROM ");
	do
	{
		if (QDialog::Accepted != d.exec())
			return;
		QSqlQuery q;
		ok=q.exec(d.query());
		if (!ok)
			ok=(QMessageBox::No==QMessageBox::critical(this, tr("SQL Error"), tr("%1\nDo you want to edit your sql again?").arg(q.lastError().text()), QMessageBox::Yes|QMessageBox::No));
	}
	while(!ok);
	QListWidgetItem * i = new QListWidgetItem();
	i->setText(text);
	queriesList->addItem(i);
	queriesList->setCurrentItem(i);
	m_queries[text]=d.query();
	removeQuery->setEnabled(queriesList->count());
}

void queryWizardPage::on_removeQuery_clicked()
{
	if (QMessageBox::Yes != QMessageBox::question(this, tr("eXaro"), tr("Do you want to remove '%1'?").arg(queriesList->currentItem()->text()), QMessageBox::Yes|QMessageBox::No))
		return;
	m_queries.remove(queriesList->currentItem()->text());
	delete queriesList->currentItem();
}

void queryWizardPage::on_editName_clicked()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Query object"), tr("query name:"), QLineEdit::Normal, queriesList->currentItem()->text(), &ok);

	if (!ok || text.isEmpty())
		return;

	QString query = m_queries[queriesList->currentItem()->text()].toString();
	m_queries.remove(queriesList->currentItem()->text());
	m_queries[text] = query;
	queriesList->currentItem()->setText(text);
}

void queryWizardPage::on_editQuery_clicked()
{
	QueryEditDialog d;
	d.setQuery(m_queries[queriesList->currentItem()->text()].toString());

	if (QDialog::Accepted == d.exec())
		m_queries[queriesList->currentItem()->text()] = d.query();
}

bool queryWizardPage::validatePage()
{
	if (!m_queries.keys().size())
		return false;
	m_report->setQueries(m_queries);
	QString script;
	foreach(QString qry, m_queries.keys())
		script+=QString("%1.exec();\n").arg(qry);
	m_report->setScript(script);
	return true;
}
