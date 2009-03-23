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
//	connect(m_editButton, SIGNAL(clicked()), this, SLOT(editItem()));
	connect(m_editName, SIGNAL(clicked()), this, SLOT(editName()));

	connect(b_dbTables, SIGNAL(toggled ( bool )), this, SLOT(fillTablesList()));
	connect(b_dbViews, SIGNAL(toggled ( bool )), this, SLOT(fillTablesList()));
	connect(b_dbSystem, SIGNAL(toggled ( bool )), this, SLOT(fillTablesList()));

	queryTable->hide();
	queryTable->setModel(&m_queryModel);

	m_dataTableModel = 0;

	resetConnection();

	QFont font("Arial", 12);
	font.setFixedPitch(true);
	editQuery->setFont(font);

	m_syntax.setDocument(editQuery->document());
	stackedWidget->setCurrentIndex(0);
}

DesignerQueryWidget::~DesignerQueryWidget()
{
}

void DesignerQueryWidget::resetConnection()
{
    if (QSqlDatabase::database().isOpen())
    {
	m_tw->setTabEnabled(1,true);
	fillTablesList();
    }
    else
    {
	qWarning("w: Could not connect to database");
	m_tw->setTabEnabled(1,false);
    }
}

void DesignerQueryWidget::fillTablesList()
{
    tablesList->clear();
    if (b_dbTables->isChecked ())
	tablesList->addItems(QSqlDatabase::database().tables(QSql::Tables));
    if (b_dbViews->isChecked ())
	tablesList->addItems(QSqlDatabase::database().tables(QSql::Views));
    if (b_dbSystem->isChecked ())
	tablesList->addItems(QSqlDatabase::database().tables(QSql::SystemTables));
}

void DesignerQueryWidget::on_b_properties_toggled ( bool checked )
{
    if (checked)
	stackedWidget->setCurrentIndex(1);
    else
	stackedWidget->setCurrentIndex(0);
}


void DesignerQueryWidget::on_tablesList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous )
{
    if (!current)
	return;
    if (m_dataTableModel)
	delete m_dataTableModel;

    m_dataTableModel = new QSqlTableModel(this);

    m_dataTableModel->setTable(current->text());
    m_dataTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_dataTableModel->select();

    dataTable->setModel(m_dataTableModel);
    dataTable->resizeColumnsToContents();
    dataTable->resizeRowsToContents();
}

void DesignerQueryWidget::on_bQueryExec_clicked()
{
    if (!QSqlDatabase::database().isOpen())
    {
	queryTable->hide();
	queryResultText->show();
	queryResultText->setPlainText("No database connection selected.\nAdd activate a connection in the left tree view.");
	return;
    }

    // initialize new sql query object
    m_queryModel.setQuery(editQuery->toPlainText());

    if (m_queryModel.lastError().isValid())
    {
	queryTable->hide();
	queryResultText->show();

	queryResultText->setPlainText(QString("%1\n%2")
				      .arg(m_queryModel.lastError().driverText())
				      .arg(m_queryModel.lastError().databaseText()));
    }
    else
    {
	// query was successful

	if (m_queryModel.query().isSelect())
	{
	    queryResultText->hide();
	    queryTable->show();
	    queryTable->resizeColumnsToContents();
	    queryTable->resizeRowsToContents();
	}
	else
	{
	    queryTable->hide();
	    queryResultText->show();

	    queryResultText->setPlainText(QString("%1 rows affected.")
					  .arg( m_queryModel.query().numRowsAffected() ));
	}
    }
}

void DesignerQueryWidget::on_m_listWidget_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous )
{
    if (!current)
	return;
    if (previous)
    {
	m_queries[previous->text()] = editQuery->toPlainText();
	//m_queries[previous->text()]
    }
    editQuery->setPlainText(m_queries[current->text()].toString());
}

/*
void DesignerQueryWidget::editItem()
{
	QueryEditDialog d;
	d.setQuery(m_queries[m_listWidget->currentItem()->text()].toString());

	if (QDialog::Accepted == d.exec())
		m_queries[m_listWidget->currentItem()->text()] = d.query();
}
*/
/*
void DesignerQueryWidget::on_m_listWidget_currentTextChanged ( const QString & currentText )
{
    QString query = m_queries[currentText].toString();
    m_queries.remove(m_listWidget->currentItem()->text());
    m_queries[currentText] = query;
    m_listWidget->currentItem()->setText(text);
}
*/

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
//	m_editButton->setEnabled(m_listWidget->count());
}


void DesignerQueryWidget::createItem()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Create an query object"), tr(" query name:"), QLineEdit::Normal, QString("query_%1").arg(m_listWidget->count()), &ok);

	if (!ok || text.isEmpty())
		return;

	QListWidgetItem * i = new QListWidgetItem();
//	i->setFlags (i->flags () | Qt::ItemIsEditable);
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
	{
	    QListWidgetItem * i = new QListWidgetItem();
//	    i->setFlags (i->flags () | Qt::ItemIsEditable);
	    i->setText(keys);
	    m_listWidget->addItem(i);
	}
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
