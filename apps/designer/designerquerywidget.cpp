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
#include "reportinterface.h"
#include "reportengine.h"

namespace Report
{

DesignerQueryWidget::DesignerQueryWidget(QWidget* parent, Qt::WFlags fl)
		: QWidget(parent, fl), Ui::designerQueryWidget(), m_report(0)
{
	setupUi(this);
	setWindowTitle(tr("Queries"));
	m_listWidget->clear();
	refreshButtons();
	connect(m_createButton, SIGNAL(clicked()), this, SLOT(createItem()));
	connect(m_deleteButton, SIGNAL(clicked()), this, SLOT(deleteItem()));
	connect(m_editName, SIGNAL(clicked()), this, SLOT(editName()));

	connect(b_dbTables, SIGNAL(toggled ( bool )), this, SLOT(fillTablesList()));
	connect(b_dbViews, SIGNAL(toggled ( bool )), this, SLOT(fillTablesList()));
	connect(b_dbSystem, SIGNAL(toggled ( bool )), this, SLOT(fillTablesList()));

	queryTable->hide();
	queryTable->setModel(&m_queryModel);

	m_dataTableModel = 0;

	resetConnection();

	QFont font("Arial", 10);
	font.setFixedPitch(true);
	editQuery->setFont(font);

	m_syntax.setDocument(editQuery->document());
	stackedWidget->setCurrentIndex(0);
}

DesignerQueryWidget::~DesignerQueryWidget()
{
}

void DesignerQueryWidget::setReport(ReportInterface * report)
{
    m_report = report;
    m_listWidget->clear();
    editQuery->clear();
    foreach(DataSet * q, m_report->datasets())
    {
	QListWidgetItem * i = new QListWidgetItem();
	i->setText(q->objectName());
	m_listWidget->addItem(i);
	m_listWidget->setCurrentItem(i);
    }

    resetConnection();
    refreshButtons();
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
    if (!current || !m_report)
	return;

    qDebug("DesignerQueryWidget::on_m_listWidget_currentItemChanged");
    if (previous)
    {
	DataSet * q = m_report->findChild<DataSet *>(previous->text());
	if (q)
	    q->setText(	editQuery->toPlainText() );
	else
	    qWarning("Cant find query named \'%s\'", qPrintable(previous->text()));
    }
    DataSet * q = m_report->findChild<DataSet *>(current->text());
    if (q)
	editQuery->setPlainText(q->text());
    else
	qWarning("Cant find query named \'%s\'", qPrintable(current->text()));
}


void DesignerQueryWidget::editName()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Query object"), tr("query name:"), QLineEdit::Normal, m_listWidget->currentItem()->text(), &ok);

    if (!ok || text.isEmpty())
	return;

    DataSet * q = m_report->findChild<DataSet *>(m_listWidget->currentItem()->text());
    if (q)
    {
	q->setObjectName(text);
	m_listWidget->currentItem()->setText(text);
    }
    else
	qWarning("Cant find query named \'%s\'", qPrintable(m_listWidget->currentItem()->text()));

}

void DesignerQueryWidget::refreshButtons()
{
	m_deleteButton->setEnabled(m_listWidget->count());
}


void DesignerQueryWidget::createItem()
{
    Q_ASSERT(m_report);
    bool ok;
    QString text = QInputDialog::getText(this, tr("Create an query object"), tr(" query name:"), QLineEdit::Normal, QString("query_%1").arg(m_listWidget->count()), &ok);

    if (!ok || text.isEmpty())
	return;

    DataSet * q = new DataSet();
    m_report->addDataset(q);
    q->setObjectName(ReportEngine::uniqueName(text, m_report));
    QListWidgetItem * i = new QListWidgetItem();
    //	i->setFlags (i->flags () | Qt::ItemIsEditable);
    i->setText(q->objectName());
    m_listWidget->addItem(i);
    m_listWidget->setCurrentItem(i);
    refreshButtons();
}

void DesignerQueryWidget::deleteItem()
{
    Q_ASSERT(m_report);
    if (QMessageBox::Ok != QMessageBox::question(this, tr("eXaro"), tr("Delete current query ?"), QMessageBox::Ok | QMessageBox::Cancel))
	return;

    DataSet * q = m_report->findChild<DataSet *>(m_listWidget->currentItem()->text());
    if (q)
    {
	delete m_listWidget->currentItem();
	delete q;
	refreshButtons();
    }
    else
	qWarning("Cant find query named \'%s\'", qPrintable(m_listWidget->currentItem()->text()));
}

void DesignerQueryWidget::sync()
{
    DataSet * q = m_report->findChild<DataSet *>(m_listWidget->currentItem()->text());
    if (q)
	q->setText( editQuery->toPlainText() );
    else
	qWarning("Cant find query named \'%s\'", qPrintable(m_listWidget->currentItem()->text()));
}

}
