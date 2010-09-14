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

#ifndef QUERYWIZARDPAGE_H
#define QUERYWIZARDPAGE_H

#include <QWizardPage>

#include "ui_querywizardpage.h"
#include "reportengine.h"

class queryWizardPage : public QWizardPage, private Ui::queryWizardPage
{
Q_OBJECT
public:
	queryWizardPage(Report::ReportEngine* reportEngine, Report::ReportInterface* reportInterface, QWidget *parent = 0);
	bool validatePage();

private slots:
	void on_setupDatabase_clicked();
	void on_addQuery_clicked();
	void on_removeQuery_clicked();
	void on_editName_clicked();
	void on_editQuery_clicked();

private:
	Report::ReportEngine* m_reportEngine;
	Report::ReportInterface* m_report;
	QMap <QString, QVariant> m_queries;

};

#endif // QUERYWIZARDPAGE_H
