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

#include "fieldswizardpage.h"
#include "pageinterface.h"

fieldsWizardPage::fieldsWizardPage(Report::ReportEngine* reportEngine, Report::ReportInterface* reportInterface, QWidget* parent):
		QWizardPage(parent), m_reportEngine(reportEngine), m_report(reportInterface)
{
	setupUi(this);
	setPixmap(QWizard::BackgroundPixmap,QPixmap(":/images/fieldsWizard.png"));
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
		QMessageBox::critical(this, tr("SQL Error"), tr("Error:\n%1").arg(q.lastError().text()));
		return;
	}

	Report::PageInterface* page=m_report->findChild<Report::PageInterface*>(pages->currentText());
	if (!page)
		return;

	Report::BandInterface* container=page->findChild<Report::BandInterface*>(detail);
	if (!container)
		return;

	QStringList groupFields; 
	foreach(Report::BandInterface* band, container->findChildren<Report::BandInterface*>())
	{
		if (!band->objectName().startsWith("DetailHeader_"))
			continue;
		groupFields.push_back(band->property("groupField").toString());
	}
	
	QStringList queryFields;
	QSqlRecord rec=q.record();
	for (int i=0;i<rec.count();i++)
	{
		if (!groupFields.contains(rec.field(i).name()) && !m_fields[QString("%1~%2").arg(pages->currentText()).arg(detail)].contains(rec.field(i).name()))
			queryFields.push_back(rec.field(i).name());
		m_fieldsType[rec.fieldName(i)]=(int)rec.field(i).type();	
	}
	foreach(QString field, queryFields)
	{
		QListWidgetItem * i = new QListWidgetItem();
		i->setText(field);
		fields->addItem(i);
	}
	fields->setCurrentRow(0);

	foreach(QString field, m_fields[QString("%1~%2").arg(pages->currentText()).arg(detail)])
	{
		QListWidgetItem * i = new QListWidgetItem();
		i->setText(field);
		onDetail->addItem(i);
		onDetail->setCurrentItem(i);
	}
	onDetail->setCurrentRow(0);
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
	fields->setCurrentRow(0);
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
	int row=onDetail->currentRow();
	if (row<=0)
		return;
	QListWidgetItem * it=onDetail->takeItem( row );
	onDetail->insertItem(row-1, it);
	onDetail->setCurrentItem(it);
	m_fields[QString("%1~%2").arg(pages->currentText()).arg(details->currentText())].swap(row,row-1);
}

void fieldsWizardPage::on_moveDown_clicked()
{
	int row=onDetail->currentRow();
	if (row>=onDetail->count()-1)
		return;
	QListWidgetItem * it=onDetail->takeItem(row);
	onDetail->insertItem(row+1, it);
	onDetail->setCurrentItem(it);
	m_fields[QString("%1~%2").arg(pages->currentText()).arg(details->currentText())].swap(row,row+1);
}

Report::ItemInterface* fieldsWizardPage::addItem(const QString& className, QObject* parent)
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


bool fieldsWizardPage::validatePage()
{
	const int fontMargin=20; //2mm
	int lineNo=0;
	foreach (QString page_query, m_fields.keys())
	{
		if (!m_fields[page_query].size())
			continue;

		Report::PageInterface* page=m_report->findChild<Report::PageInterface*>(page_query.split("~")[0]);
		int fontHeight=15*page->font().pointSizeF();

		Report::BandInterface* container=page->findChild<Report::BandInterface*>(page_query.split("~")[1]);
		QString query=container->property("query").toString();

		bool initHeader=false;
		foreach(Report::BandInterface* band, container->findChildren<Report::BandInterface*>())
		{
			if (!band->objectName().startsWith("DetailHeader") && band->objectName()!="Detail")
				continue;
			qreal bandWidth=band->width();
			qreal ypos=fontMargin;
			qreal xpos=20+fontMargin;
			qreal xstep=(bandWidth-20-(20+fontMargin)*m_fields[page_query].size())/m_fields[page_query].size();

			if (band->objectName()!="Detail" && band->property("groupField").toString().length())
			{// detail o first group
				Report::ItemInterface* script=addItem("Script", band);
				if (!script)
				{
					QMessageBox::critical(this, tr("Error"), tr("Can't find needed plugins"));
					return false;
				}
				script->setObjectName(QString("script_%1_%2").arg(query).arg(band->property("groupField").toString()));
				script->setPos(fontMargin,ypos);
				script->setHeight(fontHeight);
				script->setWidth(bandWidth-fontMargin*2);
				script->setProperty("script", QString("\"%1 :\"+%2.value('%1')").arg(band->property("groupField").toString()).arg(query));
				ypos+=fontHeight;
				if (!initHeader && drawLine->isChecked())
				{
					Report::ItemInterface* itemLine=addItem("LineItem", band);
					itemLine->setObjectName(QString("line_%1").arg(lineNo++));
					itemLine->setPos(0,ypos);
					itemLine->setProperty("lineStyle",1);
					itemLine->setWidth(band->width());
					itemLine->setHeight(20);
					ypos+=fontMargin;
				}
				ypos+=fontMargin;
			}
			
			if((band->objectName()=="DetailHeader") && drawLine->isChecked())
				band->setProperty("frame","{1,2,8}");
			else
				if((band->objectName()=="Detail") && drawLine->isChecked())
					band->setProperty("frame","{1,4,2,8}");

			if (!initHeader || band->objectName()=="Detail")
			{
				foreach(QString field, m_fields[page_query])
				{
					Report::ItemInterface* item=addItem(!initHeader?"Text":"Field", band);
					if (!item)
					{
						QMessageBox::critical(this, tr("Error"), tr("Can't find needed plugins"));
						return false;
					}
					if (!initHeader)
					{
						item->setObjectName(QString("%1_%2_text").arg(field).arg(query));
						item->setProperty("text", field);
					}
					else
					{
						item->setObjectName(QString("%1_%2_field").arg(field).arg(query));
						item->setProperty("queryName", query);
						item->setProperty("fieldName", field);
					}
					item->setPos(xpos,ypos);
					item->setHeight(fontHeight);
					item->setWidth(xstep);
					if(m_fieldsType[field] == QVariant::Double || m_fieldsType[field] == QVariant::Int)
						item->setProperty("textFlags",(int)Qt::AlignRight|Qt::AlignVCenter);
					else
						item->setProperty("textFlags",(int)Qt::AlignLeft|Qt::AlignVCenter);

					if(((xpos+xstep) <  (band->width() -20)) && (drawLine->isChecked()))
					{
						Report::ItemInterface* itemLine=addItem("LineItem", band);
						itemLine->setObjectName(QString("line_%1").arg(lineNo++));
						itemLine->setProperty("lineStyle",0);
						itemLine->setWidth(20);
						if (band->objectName()!="Detail" && band->property("groupField").toString().length())
						{
							itemLine->setPos(xpos+xstep,band->height()/2);
							itemLine->setHeight(band->height()/2);
						}
						else
						{
							itemLine->setPos(xpos+xstep,0);
							itemLine->setHeight(band->height());
						}
					}
					xpos+=(20+fontMargin)+xstep;
				}
				initHeader=true;
			}
		}
	}
	return true;
}
