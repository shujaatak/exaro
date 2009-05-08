/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
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

#include "designerquerywidget.h"
#include "queryeditdialog.h"

namespace Report
{

DesignerQueryWidget::DesignerQueryWidget(QWidget* parent, Qt::WFlags fl)
		: QWidget(parent, fl), Ui::designerQueryWidget()
{
	setupUi(this);
	setWindowTitle(tr("Queries"));
	m_listWidget->clear();
	refreshButtons();
	connect(m_createButton, SIGNAL(clicked()), this, SLOT(createItem()));
	connect(m_deleteButton, SIGNAL(clicked()), this, SLOT(deleteItem()));
	connect(m_editButton, SIGNAL(clicked()), this, SLOT(editItem()));
	connect(m_editName, SIGNAL(clicked()), this, SLOT(editName()));
}

DesignerQueryWidget::~DesignerQueryWidget()
{
}

void DesignerQueryWidget::editItem()
{
	QueryEditDialog d;
	d.setQuery(m_queries[m_listWidget->currentItem()->text()].toString());

	if (QDialog::Accepted == d.exec())
		m_queries[m_listWidget->currentItem()->text()] = d.query();
}

void DesignerQueryWidget::editName()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Query object"), tr("query name:"), QLineEdit::Normal, m_listWidget->currentItem()->text(), &ok);

	if (!ok || text.isEmpty())
		return;

	QString query = m_queries[m_listWidget->currentItem()->text()].toString();
	m_queries.remove(m_listWidget->currentItem()->text());
	m_queries[text] = query;
	m_listWidget->currentItem()->setText(text);
}

void DesignerQueryWidget::refreshButtons()
{
	m_deleteButton->setEnabled(m_listWidget->count());
	m_editButton->setEnabled(m_listWidget->count());
	m_editName->setEnabled(m_listWidget->count());
}


void DesignerQueryWidget::createItem()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Create a query object"), tr(" query name:"), QLineEdit::Normal, QString("query_%1").arg(m_listWidget->count()), &ok);

	if (!ok || text.isEmpty())
		return;

	QListWidgetItem * i = new QListWidgetItem();

	i->setText(text);

	m_listWidget->addItem(i);

	m_listWidget->setCurrentItem(i);

	refreshButtons();
}

QMap <QString, QVariant> DesignerQueryWidget::queries()
{
	return m_queries;
}

void DesignerQueryWidget::setQueries(QMap <QString, QVariant> queries)
{
	m_queries = queries;
	m_listWidget->clear();
	foreach(QString keys, m_queries.keys())
		m_listWidget->addItem(keys);

	if (m_listWidget->count())
		m_listWidget->setCurrentRow(0);

	refreshButtons();
}

void DesignerQueryWidget::deleteItem()
{

	if (QMessageBox::Ok != QMessageBox::question(this, tr("eXaro"), tr("Delete current query ?"), QMessageBox::Ok | QMessageBox::Cancel))
		return;

	m_queries.remove(m_listWidget->currentItem()->text());

	delete m_listWidget->currentItem();

	refreshButtons();
}

}
