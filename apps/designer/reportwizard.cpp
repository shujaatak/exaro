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
#include "groupwizardpage.h"
#include "fieldswizardpage.h"
#include <QMessageBox>

reportWizard::reportWizard(Report::ReportEngine * reportEngine, QWidget* parent):QWizard(parent), m_reportEngine(reportEngine), m_finished(false)
{
	setWizardStyle(MacStyle);

#ifdef __GNUC__
#warning "FIXME <Back> button is disabled for the moment, enable it when all pages are properly initializated" 
#else
#pragma WARNING( FIXME <Back> button is disabled for the moment: enable it when all pages are properly initializated )
#endif
	QList<QWizard::WizardButton> layout;
//	layout << QWizard::HelpButton << QWizard::Stretch << QWizard::CancelButton << QWizard::NextButton<< QWizard::FinishButton;
	layout << QWizard::Stretch << QWizard::CancelButton << QWizard::NextButton<< QWizard::FinishButton;
	setButtonLayout(layout);
	setWindowTitle(tr("Report wizard"));

	m_report=0;
	m_report= m_reportEngine->reports()[0]->createInstance(0);
	m_report->setObjectName( "report" );
	m_report->setName( tr( "Report name" ) );
	m_report->setAuthor( "(c) 2009 BogDan" );
	addPage(createIntroPage());
	addPage(new queryWizardPage(m_reportEngine,m_report));
	addPage(new pageWizardPage(m_reportEngine,m_report));
	addPage(new groupWizardPage(m_reportEngine,m_report));
	addPage(new fieldsWizardPage(m_reportEngine,m_report));
	addPage(createFinalPage());
}

QWizardPage * reportWizard::createIntroPage()
{
	QWizardPage *page = new QWizardPage;
	page->setTitle(tr("Welcome to eXaro report wizard"));
	page->setPixmap(QWizard::BackgroundPixmap,QPixmap(":/images/startWizard.png"));//.scaled(width(),height()));

	QLabel *label = new QLabel(tr("This wizard will help you create reports fast and easily.<br>If you want only an empty report, press <b>cancel</b> button."));
	label->setWordWrap(true);
	label->setTextFormat(Qt::RichText);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	page->setLayout(layout);
	return page;
}

QWizardPage * reportWizard::createFinalPage()
{
	QWizardPage *page = new QWizardPage;
	page->setTitle(tr("Congratulations"));
	page->setPixmap(QWizard::BackgroundPixmap,QPixmap(":/images/finishWizard.png"));//.scaled(width(),height()));

	QLabel *label = new QLabel(tr("Congratulations you have completed the wizard.<br>Press <b>done</b> button to enjoy your report !"));
	label->setWordWrap(true);
	label->setTextFormat(Qt::RichText);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	page->setLayout(layout);
	return page;
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

void reportWizard::reject()
{
	if (QMessageBox::question(this, tr("Close wizard?"), tr("Do you want to close the wizard?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		QDialog::reject();
}
