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

#ifndef REPORTWIZARD_H
#define REPORTWIZARD_H

#include <QWizard>
#include "reportengine.h"
#include "reportinterface.h"

class reportWizard : public QWizard
{
Q_OBJECT
public:
	enum {Page_Query, Page_Page, Page_Group, Page_Field};

public:
	reportWizard(Report::ReportEngine * reportEngine, QWidget *parent = 0);
	~reportWizard();
	void accept();
	void reject();
	Report::ReportInterface * report();
	QWizardPage * createIntroPage();
	QWizardPage * createFinalPage();

private:
	Report::ReportEngine * m_reportEngine;
	Report::ReportInterface * m_report;
	bool m_finished;
};

#endif // REPORTWIZARD_H
