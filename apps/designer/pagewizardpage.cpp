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

#include <QMetaObject>
#include <QMetaProperty>

#include "pagewizardpage.h"
#include "pageinterface.h"

pageWizardPage::pageWizardPage(Report::ReportEngine* reportEngine, Report::ReportInterface* reportInterface, QWidget* parent):
		QWizardPage(parent), m_reportEngine(reportEngine), m_report(reportInterface)
{
	setupUi(this);
	setPixmap(QWizard::BackgroundPixmap,QPixmap(":/images/pageWizard.png"));
	removePage->setEnabled(pagesList->count());
}

void pageWizardPage::on_addPage_clicked()
{
	Report::PageInterface* page=reinterpret_cast<Report::PageInterface*>(m_reportEngine->pages()[0]->createInstance( m_report ));
	page->setObjectName(QString("page_%1").arg(m_report->findChildren<Report::PageInterface*>().size()));
	QListWidgetItem * i = new QListWidgetItem();
	i->setText(page->objectName());
	pagesList->addItem(i);
	pagesList->setCurrentItem(i);
	removePage->setEnabled(pagesList->count());
	if (pagesList->count()==1)
	{
		pageSize->clear();
		for (int en = 0;en < page->metaObject()->enumerator(page->metaObject()->indexOfEnumerator(page->metaObject()->property(page->metaObject()->indexOfProperty("pageSize")).typeName())).keyCount();en++)
			pageSize->insertItem(en, page->metaObject()->enumerator(page->metaObject()->indexOfEnumerator(page->metaObject()->property(page->metaObject()->indexOfProperty("pageSize")).typeName())).key(en), page->metaObject()->enumerator(page->metaObject()->indexOfEnumerator(page->metaObject()->property(page->metaObject()->indexOfProperty("pageSize")).typeName())).value(en));
		pageSize->findData(page->pageSize());

		pageOrientation->clear();
		for (int en = 0;en < page->metaObject()->enumerator(page->metaObject()->indexOfEnumerator(page->metaObject()->property(page->metaObject()->indexOfProperty("orientation")).typeName())).keyCount();en++)
			pageOrientation->insertItem(en, page->metaObject()->enumerator(page->metaObject()->indexOfEnumerator(page->metaObject()->property(page->metaObject()->indexOfProperty("orientation")).typeName())).key(en), page->metaObject()->enumerator(page->metaObject()->indexOfEnumerator(page->metaObject()->property(page->metaObject()->indexOfProperty("orientation")).typeName())).value(en));
		pageOrientation->findData(page->orientation());
	}
}

void pageWizardPage::on_removePage_clicked()
{
	delete m_report->findChildren<Report::PageInterface*>()[pagesList->currentRow()];
	delete pagesList->currentItem();
	removePage->setEnabled(pagesList->count());
}

void pageWizardPage::on_pagesList_currentRowChanged(int row)
{
	if (row<0 || row>=m_report->findChildren<Report::PageInterface*>().size())
		return;
	Report::PageInterface* page=m_report->findChildren<Report::PageInterface*>()[row];
	pageSize->setCurrentIndex(pageSize->findData(page->pageSize()));
	pageOrientation->setCurrentIndex(pageOrientation->findData(page->orientation()));
}

void pageWizardPage::on_pageSize_currentIndexChanged(int item)
{
	int row=pagesList->currentRow();
	if (row<0 || row>=m_report->findChildren<Report::PageInterface*>().size())
		return;
	Report::PageInterface* page=m_report->findChildren<Report::PageInterface*>()[row];
	page->setPageSize((Report::PageInterface::PageSize)pageSize->itemData(item).toInt());
}

void pageWizardPage::on_pageOrientation_currentIndexChanged(int item)
{
	int row=pagesList->currentRow();
	if (row<0 || row>=m_report->findChildren<Report::PageInterface*>().size())
		return;
	Report::PageInterface* page=m_report->findChildren<Report::PageInterface*>()[row];
	page->setOrientation((Report::PageInterface::Orientation)pageOrientation->itemData(item).toInt());
}


bool pageWizardPage::validatePage()
{
	return pagesList->count();
}

