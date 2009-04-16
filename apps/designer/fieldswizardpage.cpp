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
#include <QFont>
#include <QSqlRecord>
#include <QSqlField>

#include <QDebug>

#include "fieldswizardpage.h"
#include "pageinterface.h"

fieldsWizardPage::fieldsWizardPage(Report::ReportEngine* reportEngine, Report::ReportInterface* reportInterface, QWidget* parent):
		QWizardPage(parent), m_reportEngine(reportEngine), m_report(reportInterface)
{
	setupUi(this);
}

void fieldsWizardPage::initializePage()
{
	setButtonsStartus();
	pages->clear();
	foreach(Report::PageInterface* page, m_report->findChildren<Report::PageInterface*>())
		pages->addItem(page->objectName());
}

void fieldsWizardPage::setButtonsStartus()
{
	add->setEnabled(fields->count());
	addAll->setEnabled(fields->count());
	remove->setEnabled(onDetail->count());
	removeAll->setEnabled(onDetail->count());
	moveUp->setEnabled(onDetail->count());
	moveDown->setEnabled(onDetail->count());
}


void fieldsWizardPage::updateGroupList()
{
/*	groupsList->clear();
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
	removeGroup->setEnabled(groupsList->count());*/
}

void fieldsWizardPage::on_pages_currentIndexChanged(const QString& pageName)
{
	fields->clear();
	details->clear();
	Report::PageInterface* page=m_report->findChild<Report::PageInterface*>(pageName);
	if (!page)
		return;
	foreach(Report::BandInterface* band, page->findChildren<Report::BandInterface*>())
	{
		if (!band->objectName().startsWith("DetailContainer"))
			continue;
		details->addItem(band->objectName());
	}
	setButtonsStartus();
}

void fieldsWizardPage::on_details_currentIndexChanged(const QString& detail)
{
	fields->clear();
	onDetail->clear();
	QString query=detail.mid(detail.indexOf("_")+1);
	QSqlQuery q;
	if (!q.exec(m_report->queries()[query].toString()))
	{
		QMessageBox::critical(this, tr("SQL Error"), tr("%1").arg(q.lastError().text()));
		return;
	}
	QStringList queryFields;
	QSqlRecord rec=q.record();
	for (int i=0;i<rec.count();i++)
		if (!m_fields[QString("%1~%2").arg(pages->currentText()).arg(detail)].contains(rec.field(i).name()))
			queryFields.push_back(rec.field(i).name());

	foreach(QString field, queryFields)
	{
		QListWidgetItem * i = new QListWidgetItem();
		i->setText(field);
		fields->addItem(i);
		fields->setCurrentItem(i);
	}

	foreach(QString field, m_fields[QString("%1~%2").arg(pages->currentText()).arg(detail)])
	{
		QListWidgetItem * i = new QListWidgetItem();
		i->setText(field);
		onDetail->addItem(i);
		onDetail->setCurrentItem(i);
	}
	setButtonsStartus();
}

void fieldsWizardPage::on_add_clicked()
{
	QListWidgetItem * it = fields->currentItem();
	if (!it)
		return;
	QListWidgetItem * id= new QListWidgetItem();
	id->setText(it->text());
	onDetail->addItem(id);
	onDetail->setCurrentItem(id);
	m_fields[QString("%1~%2").arg(pages->currentText()).arg(details->currentText())].push_back(it->text());
	delete it;
	setButtonsStartus();
}

void fieldsWizardPage::on_addAll_clicked()
{
	while(fields->currentItem())
	{
		QListWidgetItem * it = fields->currentItem();
		QListWidgetItem * id= new QListWidgetItem();
		id->setText(it->text());
		onDetail->addItem(id);
		onDetail->setCurrentItem(id);
		m_fields[QString("%1~%2").arg(pages->currentText()).arg(details->currentText())].push_back(it->text());
		delete it;
	}
	setButtonsStartus();
}

void fieldsWizardPage::on_remove_clicked()
{
	QListWidgetItem * it = onDetail->currentItem();
	if (!it)
		return;
	QListWidgetItem * id= new QListWidgetItem();
	id->setText(it->text());
	fields->addItem(id);
	fields->setCurrentItem(id);
	m_fields[QString("%1~%2").arg(pages->currentText()).arg(details->currentText())].removeAt(m_fields[QString("%1~%2").arg(pages->currentText()).arg(details->currentText())].indexOf(it->text()));
	delete it;
	setButtonsStartus();
}

void fieldsWizardPage::on_removeAll_clicked()
{
	while(onDetail->currentItem())
	{
		QListWidgetItem * it = onDetail->currentItem();
		if (!it)
			return;
		QListWidgetItem * id= new QListWidgetItem();
		id->setText(it->text());
		fields->addItem(id);
		fields->setCurrentItem(id);
		m_fields[QString("%1~%2").arg(pages->currentText()).arg(details->currentText())].removeAt(m_fields[QString("%1~%2").arg(pages->currentText()).arg(details->currentText())].indexOf(it->text()));
		delete it;
	}
	setButtonsStartus();
}

void fieldsWizardPage::on_moveUp_clicked()
{
//insertItem
}

void fieldsWizardPage::on_moveDown_clicked()
{

}

/*
void fieldsWizardPage::on_addGroup_clicked()
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

void fieldsWizardPage::on_removeGroup_clicked()
{
	m_groups[QString("%1~%2").arg(pages->currentText()).arg(queries->currentText())].removeAt ( m_groups[QString("%1~%2").arg(pages->currentText()).arg(queries->currentText())].indexOf(groupsList->currentItem()->text()));
	updateGroupList();
}
*/

Report::ItemInterface* fieldsWizardPage::addItem(const QString& className, QObject* parent)
{
/*	Report::ItemInterface* itemExample = m_reportEngine->findItemByClassName( className );

	Report::ItemInterface *m_item = 0;

	if ( dynamic_cast<Report::ItemInterface*>( parent ) )
	{
		if ( dynamic_cast<Report::ItemInterface*>( parent )->canContain( itemExample ) )
			m_item = dynamic_cast<Report::ItemInterface*>( itemExample->createInstance( dynamic_cast<QGraphicsItem*>( parent ), parent ) );
	}
	else
		if ( dynamic_cast<Report::PageInterface*>( parent )->canContain( itemExample ) )
			dynamic_cast<Report::PageInterface*>( parent )->addItem( m_item = dynamic_cast<Report::ItemInterface*>( itemExample->createInstance( 0, parent ) ) );
	return m_item;*/
}


bool fieldsWizardPage::validatePage()
{
/*	const int fontMargin=20; //2mm
	foreach (QString page_query, m_groups.keys())
	{
		if (!m_groups[page_query].size())
			continue;

		Report::PageInterface* page=m_report->findChild<Report::PageInterface*>(page_query.split("~")[0]);
		int fontHeight=10*page->font().pointSizeF();

		if (!page->findChildren<Report::BandInterface*>().size())
		{
			Report::ItemInterface* title=addItem("Title", page);
			if (title)
			{
				title->setObjectName(QString("titile_%1").arg(page_query.split("~")[0]));
				title->setHeight(150);
				title->setProperty("order", 0);
//#warning TODO: here we can add more item to this band, like a text item, and maybe 2 lines !!!
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
		detailContainer->setProperty("query", query);
		detailContainer->setHeight(((int)(!detailOnFirstGroup->isChecked())+(int)generateDetailFooters->isChecked()*m_groups[page_query].size()+2+m_groups[page_query].size())*(2*fontMargin+fontHeight));
		detailContainer->setProperty("order", 0);

		int order=0;

		if (!detailOnFirstGroup->isChecked())
		{
			Report::ItemInterface* detailHeader=addItem("DetailHeader", detailContainer);
			if (!detailHeader)
			{
				QMessageBox::critical(this, tr("Error"), tr("Can't find needed plugins"));
				return false;
			}
			detailHeader->setObjectName("DetailHeader");
			detailHeader->setHeight(2*fontMargin+fontHeight);
			detailHeader->setProperty("reprintOnNewPage", true);
			detailHeader->setProperty("order", order++);
			detailHeader->setProperty("order", -1);
		}

		foreach(QString groupField, m_groups[page_query])
		{
			Report::ItemInterface* detailHeader=addItem("DetailHeader", detailContainer);
			if (!detailHeader)
			{
				QMessageBox::critical(this, tr("Error"), tr("Can't find needed plugins"));
				return false;
			}
			detailHeader->setObjectName(QString("DetailHeader_%1").arg(groupField));
			detailHeader->setProperty("groupField",groupField);
			detailHeader->setHeight(2*fontMargin+fontHeight);
			if (!order && detailOnFirstGroup->isChecked())
				detailHeader->setProperty("reprintOnNewPage", true);
			detailHeader->setProperty("order", order++);

			if (generateDetailFooters->isChecked())
			{
				Report::ItemInterface* detailFooter=addItem("DetailFooter", detailContainer);
				if (!detailFooter)
				{
					QMessageBox::critical(this, tr("Error"), tr("Can't find needed plugins"));
					return false;
				}
				detailFooter->setObjectName(QString("DetailFooter_%1").arg(groupField));
				detailFooter->setProperty("groupField",groupField);
				detailFooter->setHeight(2*fontMargin+fontHeight);
				detailFooter->setProperty("order", m_groups[page_query].size());
				detailFooter->setProperty("order", -1);
			}
		}

		Report::ItemInterface* detail=addItem("Detail", detailContainer);
		if (!detail)
		{
			QMessageBox::critical(this, tr("Error"), tr("Can't find needed plugins"));
			return false;
		}
		detail->setObjectName("Detail");
		detail->setHeight(2*fontMargin+fontHeight);
		detail->setProperty("order", 0);
	}*/
	return true;
}
