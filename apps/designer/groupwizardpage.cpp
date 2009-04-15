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
#include <QSqlRecord>
#include <QSqlField>

#include <QDebug>

#include "groupwizardpage.h"
#include "pageinterface.h"

groupWizardPage::groupWizardPage(Report::ReportEngine* reportEngine, Report::ReportInterface* reportInterface, QWidget* parent):
		QWizardPage(parent), m_reportEngine(reportEngine), m_report(reportInterface)
{
	setupUi(this);
	connect(pages, SIGNAL(currentIndexChanged(int)), SLOT(updateGroupList()));
	connect(queries, SIGNAL(currentIndexChanged(int)), SLOT(updateGroupList()));
}

void groupWizardPage::initializePage()
{
	pages->clear();
	queries->clear();
	foreach(Report::PageInterface* page, m_report->findChildren<Report::PageInterface*>())
		pages->addItem(page->objectName());
	foreach(QString query, m_report->queries().keys())
		queries->addItem(query);
}

void groupWizardPage::updateGroupList()
{
	groupsList->clear();
	QListWidgetItem * i=0;
	foreach(QString group, m_groups[QString("%1~%2").arg(pages->currentText()).arg(queries->currentText())])
	{
		i = new QListWidgetItem();
		i->setText(group);
		groupsList->addItem(i);
		groupsList->setCurrentItem(i);
	}
	if (i)
		i->setSelected(true);
	removeGroup->setEnabled(groupsList->count());
}

void groupWizardPage::on_addGroup_clicked()
{
	QSqlQuery q;
	if (!q.exec(m_report->queries()[queries->currentText()].toString()))
	{
		QMessageBox::critical(this, tr("SQL Error"), tr("%1").arg(q.lastError().text()));
		return;
	}
	QStringList fields;
	QSqlRecord rec=q.record();
	for (int i=0;i<rec.count();i++)
		if (!m_groups[QString("%1~%2").arg(pages->currentText()).arg(queries->currentText())].contains(rec.field(i).name()))
			fields.push_back(rec.field(i).name());
	if (!fields.size())
		return;
	bool ok;
	QString field=QInputDialog::getItem(this, tr("Choose a field"), tr("Group field"), fields, 0, false, &ok);
	if (ok)
		m_groups[QString("%1~%2").arg(pages->currentText()).arg(queries->currentText())].push_back(field);
	updateGroupList();
}

void groupWizardPage::on_removeGroup_clicked()
{
	m_groups[QString("%1~%2").arg(pages->currentText()).arg(queries->currentText())].removeAt ( m_groups[QString("%1~%2").arg(pages->currentText()).arg(queries->currentText())].indexOf(groupsList->currentItem()->text()));
	updateGroupList();
}

Report::ItemInterface* groupWizardPage::addItem(const QString& className, QObject* parent)
{
	Report::ItemInterface* itemExample = m_reportEngine->findItemByClassName( className );

	Report::ItemInterface *m_item = 0;

	if ( dynamic_cast<Report::ItemInterface*>( parent ) )
	{
		if ( dynamic_cast<Report::ItemInterface*>( parent )->canContain( itemExample ) )
			m_item = dynamic_cast<Report::ItemInterface*>( itemExample->createInstance( dynamic_cast<QGraphicsItem*>( parent ), parent ) );
	}
	else
		if ( dynamic_cast<Report::PageInterface*>( parent )->canContain( itemExample ) )
			dynamic_cast<Report::PageInterface*>( parent )->addItem( m_item = dynamic_cast<Report::ItemInterface*>( itemExample->createInstance( 0, parent ) ) );
	return m_item;
}


bool groupWizardPage::validatePage()
{
	foreach (QString page_query, m_groups.keys())
	{
		if (!m_groups[page_query].size())
			continue;
		Report::PageInterface* page=m_report->findChild<Report::PageInterface*>(page_query.split("~")[0]);
		if (!page->findChildren<Report::BandInterface*>().size())
		{
			Report::ItemInterface* item=addItem("Title", page);
			if (item)
			{
				item->setObjectName(QString("titile_%1").arg(page_query.split("~")[0]));
#warning TODO: here we can add more item to this band, like a text item, and maybe 2 lines !!!
			}
		}
	
		Report::ItemInterface* detailContainer=addItem("DetailContainer", page);
		if (!detailContainer)
		{
			QMessageBox::critical(this, tr("Error"), tr("Can't find needed plugins"));
			return false;
		}
		QString query=page_query.split("~")[1];
		detailContainer->setObjectName(QString("DetailContainer_%1").arg(query));
		foreach(QString groupField, m_groups[page_query])
		{
			
		}
	}
	return true;
}
