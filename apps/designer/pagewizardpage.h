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

#ifndef PAGEWIZARDPAGE_H
#define PAGEWIZARDPAGE_H

#include <QWizardPage>

#include "ui_pagewizardpage.h"
#include "reportengine.h"

class pageWizardPage:public QWizardPage, private Ui::pageWizardPage
{
Q_OBJECT
public:
	pageWizardPage(Report::ReportEngine* reportEngine, Report::ReportInterface* reportInterface, QWidget *parent = 0);
	bool validatePage();

private slots:
	void on_addPage_clicked();
	void on_removePage_clicked();
	void on_pagesList_currentRowChanged(int row);
	void on_pageSize_currentIndexChanged(int item);
	void on_pageOrientation_currentIndexChanged(int item);

private:
	Report::ReportEngine* m_reportEngine;
	Report::ReportInterface* m_report;
};

#endif // PAGEWIZARDPAGE_H
