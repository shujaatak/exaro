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

#ifndef GROUPWIZARDPAGE_H
#define GROUPWIZARDPAGE_H

#include <QWizardPage>
#include <QStringList>

#include "ui_groupwizardpage.h"
#include "reportengine.h"
#include "iteminterface.h"


class groupWizardPage:public QWizardPage, private Ui::groupWizardPage
{
Q_OBJECT
public:
	groupWizardPage(Report::ReportEngine* reportEngine, Report::ReportInterface* reportInterface, QWidget *parent = 0);
	void initializePage();
	Report::ItemInterface * addItem(const QString &className, QObject * parent);
	bool validatePage();

private slots:
	void on_addGroup_clicked();
	void on_removeGroup_clicked();
	void updateGroupList();
private:
	Report::ReportEngine* m_reportEngine;
	Report::ReportInterface* m_report;
	QMap<QString, QStringList > m_groups;
};

#endif
