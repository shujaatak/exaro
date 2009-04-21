/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by Mikhalov Alexander                              *
 *   alexmi3@rambler.ru                                                    *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#include <QStackedWidget>

#include "designerdataseteditor.h"
#include "reportinterface.h"
#include "reportengine.h"
#include "dataset.h"
#include "dataseteditor.h"
#include "newdatasetdialog.h"

namespace Report
{

DesignerDatasetEditor::DesignerDatasetEditor(ReportEngine * engine, QWidget* parent, Qt::WFlags fl)
		: QWidget(parent, fl), Ui::DesignerDatasetEditor(), m_report(0), m_engine(engine)
{
	setupUi(this);
	setWindowTitle(tr("Data"));

	m_listWidget->clear();
	refreshButtons();
	connect(m_createButton, SIGNAL(clicked()), this, SLOT(createItem()));
	connect(m_deleteButton, SIGNAL(clicked()), this, SLOT(deleteItem()));
	connect(m_editName, SIGNAL(clicked()), this, SLOT(editName()));

	connect(b_dbTables, SIGNAL(toggled ( bool )), this, SLOT(fillTablesList()));
	connect(b_dbViews, SIGNAL(toggled ( bool )), this, SLOT(fillTablesList()));
	connect(b_dbSystem, SIGNAL(toggled ( bool )), this, SLOT(fillTablesList()));

	datasetTable->hide();

	m_dataTableModel = 0;

	resetConnection();

//	QFont font("Arial", 10);
//	font.setFixedPitch(true);
//	editQuery->setFont(font);

	stackedWidget->setCurrentIndex(0);
	currentEditor = 0;
}

DesignerDatasetEditor::~DesignerDatasetEditor()
{
    foreach (DataSetEditor* w, m_editors)
	delete w;
}

void DesignerDatasetEditor::setReport(ReportInterface * report)
{
    m_report = report;

    if (currentEditor)
	gridLayoutEditors->removeWidget(currentEditor);
    currentEditor = 0;

    foreach (DataSetEditor* w, m_editors)
	delete w;
    m_editors.clear();

    m_listWidget->clear();

    foreach(DataSet * dtst, m_report->datasets())
    {
	QListWidgetItem * i = new QListWidgetItem();
	i->setText(dtst->objectName());
	m_listWidget->addItem(i);
	m_listWidget->setCurrentItem(i);

	if (!m_editors.contains(dtst->metaObject()->className()))
	{
	    DataSetEditor * ed = dtst->createEditor();
	    if (!ed)
		ed = new DataSetEditor(this);
	    m_editors.insertMulti(dtst->metaObject()->className(), ed);
	}
	if (!currentEditor)
	{
	    currentEditor = m_editors.value(dtst->metaObject()->className());
	    currentEditor->setDataset(dtst);
	    gridLayoutEditors->addWidget( currentEditor );
	}
    }

    resetConnection();
    refreshButtons();
}

void DesignerDatasetEditor::resetConnection()
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

void DesignerDatasetEditor::fillTablesList()
{
    tablesList->clear();
    if (b_dbTables->isChecked ())
	tablesList->addItems(QSqlDatabase::database().tables(QSql::Tables));
    if (b_dbViews->isChecked ())
	tablesList->addItems(QSqlDatabase::database().tables(QSql::Views));
    if (b_dbSystem->isChecked ())
	tablesList->addItems(QSqlDatabase::database().tables(QSql::SystemTables));
}

void DesignerDatasetEditor::on_b_properties_toggled ( bool checked )
{
    if (checked)
	stackedWidget->setCurrentIndex(1);
    else
	stackedWidget->setCurrentIndex(0);
}


void DesignerDatasetEditor::on_tablesList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous )
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

void DesignerDatasetEditor::on_bDatasetExec_clicked()
{
    qDebug("DesignerDatasetEditor::on_bDatasetExec_clicked()");
    if (!currentEditor)
	return;

    DataSet * dtst = m_report->findChild<DataSet *>(m_listWidget->currentItem()->text());
    Q_ASSERT(dtst);

    currentEditor->sync();

    if (!dtst->populate())
    {
	qWarning("Error while populate dataset");
	datasetTable->hide();
	datasetResultText->show();
	datasetResultText->setPlainText(dtst->lastError());
	return;
    }

    qDebug("size of dataset = %i", dtst->size());
    datasetTable->setModel(0);			// don't know why but view not update without this
    datasetTable->setModel(dtst->model());
    datasetResultText->hide();
    datasetTable->show();
    datasetTable->resizeColumnsToContents();
    datasetTable->resizeRowsToContents();
}

void DesignerDatasetEditor::on_m_listWidget_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous )
{
    qDebug("DesignerDatasetEditor::on_m_listWidget_currentItemChanged ");
    if (!current || !m_report)
	return;

    if (previous && currentEditor)
	currentEditor->sync();

    DataSet * dtst = m_report->findChild<DataSet *>(current->text());
    Q_ASSERT(dtst);

    if ( currentEditor != m_editors.value(dtst->metaObject()->className()) )
    {
	if (currentEditor)
	{
	    gridLayoutEditors->removeWidget(currentEditor);
	    currentEditor->hide();
	}
	currentEditor = m_editors.value(dtst->metaObject()->className());
	if (currentEditor)
	{
	    gridLayoutEditors->addWidget( currentEditor );
	    currentEditor->show();
	}
    }

    if (currentEditor)
    {
	currentEditor->setDataset(dtst);
	datasetTable->setModel(dtst->model());
	datasetTable->resizeColumnsToContents();
	datasetTable->resizeRowsToContents();
    }
}


void DesignerDatasetEditor::editName()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Dataset object"), tr("dataset name:"), QLineEdit::Normal, m_listWidget->currentItem()->text(), &ok);

    if (!ok || text.isEmpty())
	return;

    DataSet * dtst = m_report->findChild<DataSet *>(m_listWidget->currentItem()->text());
    if (dtst)
    {
	dtst->setObjectName(text);
	m_listWidget->currentItem()->setText(text);
    }
    else
	qWarning("Cant find dataset named \'%s\'", qPrintable(m_listWidget->currentItem()->text()));

}

void DesignerDatasetEditor::refreshButtons()
{
	m_deleteButton->setEnabled(m_listWidget->count());
}

void DesignerDatasetEditor::createItem()
{
    Q_ASSERT(m_report);

    bool ok;

    NewDatasetDialog d;
    foreach (Report::DataSet* dtst, m_engine->datasets())
	d.addType(dtst->metaObject()->className());
    d.setName(QString("ds_%1").arg(m_listWidget->count()));
    if (!d.exec())
	return;

    Report::DataSet * plugin = dynamic_cast<Report::DataSet *>(m_engine->findDatasetByClassName( d.type() ));
    if (plugin == 0 )
	return;

    Report::DataSet * dtst = dynamic_cast<Report::DataSet *> (plugin->createInstance(m_report));
    m_report->addDataset(dtst);
    dtst->setObjectName(ReportEngine::uniqueName(d.name(), m_report));

    if (!m_editors.contains(dtst->metaObject()->className()))
    {
	DataSetEditor * ed = dtst->createEditor();
	if (!ed)
	    ed = new DataSetEditor(this);
	m_editors.insertMulti(dtst->metaObject()->className(), ed);
    }

    QListWidgetItem * i = new QListWidgetItem();
    //	i->setFlags (i->flags () | Qt::ItemIsEditable);
    i->setText(dtst->objectName());
    m_listWidget->addItem(i);
    m_listWidget->setCurrentItem(i);
    if (currentEditor)
	gridLayoutEditors->removeWidget(currentEditor);

    Q_ASSERT (m_editors.value(dtst->metaObject()->className(),0) == 0);

    currentEditor = m_editors.value(dtst->metaObject()->className());
    currentEditor->setDataset(dtst);
    gridLayoutEditors->addWidget( currentEditor );
    refreshButtons();
}

void DesignerDatasetEditor::deleteItem()
{
    Q_ASSERT(m_report);
    if (QMessageBox::Ok != QMessageBox::question(this, tr("eXaro"), tr("Delete current query ?"), QMessageBox::Ok | QMessageBox::Cancel))
	return;

    DataSet * dtst = m_report->findChild<DataSet *>(m_listWidget->currentItem()->text());
    if (dtst)
    {
	delete m_listWidget->currentItem();
	delete dtst;
	refreshButtons();
    }
    else
	qWarning("Cant find dataset named \'%s\'", qPrintable(m_listWidget->currentItem()->text()));
}

void DesignerDatasetEditor::sync()
{
    if (currentEditor)
	currentEditor->sync();
}

}
