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
#include <QtGui>
#include "csvdataseteditor.h"
#include "csvdataset.h"
#include "ui_csvdataseteditor.h"

CsvDatasetEditor::CsvDatasetEditor(QWidget *parent) :
    DataSetEditor(parent),
    m_ui(new Ui::CsvDatasetEditor)
{
    m_ui->setupUi(this);
}

CsvDatasetEditor::~CsvDatasetEditor()
{
    delete m_ui;
}

void CsvDatasetEditor::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CsvDatasetEditor::setDataset(Report::DataSet* dtst)
{
    Q_ASSERT(dtst);

    if (! dynamic_cast<CsvDataset*> (dtst))
	return;

    m_dataset = dtst;
    // restore to form
    m_ui->leDelimeter->setText(dynamic_cast<CsvDataset*> (dtst)->delimeter());
    m_ui->leFileName->setText( dynamic_cast<CsvDataset*> (dtst)->fileName());
}

void CsvDatasetEditor::sync()
{
    if (!dynamic_cast<CsvDataset*> (m_dataset))
	return;

    dynamic_cast<CsvDataset*> (m_dataset)->setDelimeter(m_ui->leDelimeter->text());
    dynamic_cast<CsvDataset*> (m_dataset)->setFileName(m_ui->leFileName->text());
}

void CsvDatasetEditor::on_bBrowse_clicked()
{
    if (!m_dataset)
	return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("CSV Files (*.csv *.txt)"));
    if (fileName.isEmpty())
	return;
    dynamic_cast<CsvDataset*> (m_dataset)->setFileName(fileName);
    m_ui->leFileName->setText(fileName);
}

void CsvDatasetEditor::on_bInject_clicked()
{
    if (!m_dataset || m_ui->leFileName->text().isEmpty())
    {
	QMessageBox::warning(this, tr("DataSet"),
				tr("Nothing to be injected. Please select file first"),QMessageBox::Ok, QMessageBox::Ok);
	return;
    }

    QStringList list;
    QFile data(m_ui->leFileName);
    if (data.open(QFile::ReadOnly | QFile::Text)) {
	QTextStream in(&data);
	do {
	    list.append(in.readLine());
	} while (!in.atEnd() );
    }

    dynamic_cast<CsvDataset*> (m_dataset)->setList(list);
    m_ui->leFileName->clear();
    dynamic_cast<CsvDataset*> (m_dataset)->setFileName("");
}
