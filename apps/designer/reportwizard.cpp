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
#include "reportwizard.h"
#include "querywizardpage.h"
#include "pagewizardpage.h"

reportWizard::reportWizard(Report::ReportEngine * reportEngine, QWidget* parent):QWizard(parent), m_reportEngine(reportEngine), m_finished(false)
{
	setOption(QWizard::HaveFinishButtonOnEarlyPages);
	setWindowTitle(tr("Report wizard"));
	m_report=0;
	m_report= m_reportEngine->reports()[0]->createInstance(0);
	m_report->setObjectName( "report" );
	m_report->setName( tr( "Report name" ) );
	m_report->setAuthor( "(c) 2009 BogDan" );
	addPage(new queryWizardPage(m_reportEngine,m_report));
	addPage(new pageWizardPage(m_reportEngine,m_report));
//	setPage(Page_Query, new queryWizardPage(m_reportEngine,m_report));
}

reportWizard::~reportWizard()
{
	if (!m_finished)
		delete m_report;
}

Report::ReportInterface* reportWizard::report() 
{
	if (m_finished)
		return m_report;
	else
		return 0;
}

void reportWizard::accept()
{
	m_finished=true;
	QDialog::accept();
}

