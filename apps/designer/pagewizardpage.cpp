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

#include "pagewizardpage.h"

pageWizardPage::pageWizardPage(Report::ReportEngine* reportEngine, Report::ReportInterface* reportInterface, QWidget* parent):
		QWizardPage(parent), m_reportEngine(reportEngine), m_report(reportInterface)
{
	setupUi(this);
	removePage->setEnabled(pagesList->count());
}

void pageWizardPage::on_addPage_clicked()
{
}

void pageWizardPage::on_removePage_clicked()
{
/*
	if (QMessageBox::Yes != QMessageBox::question(this, tr("eXaro"), tr("Do you want to remove '%1'").arg(pagesList->currentItem()->text()), QMessageBox::Yes|QMessageBox::No))
		return;
	delete pagesList->currentItem();
*/
}

bool pageWizardPage::validatePage()
{
//	m_report->setQueries(m_queries);
}
