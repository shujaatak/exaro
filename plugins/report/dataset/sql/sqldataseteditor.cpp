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

#include "sqldataseteditor.h"
#include "sqldataset.h"
#include "ui_sqldataseteditor.h"
#include "sqlsyntaxhighlighter.h"

SqlDatasetEditor::SqlDatasetEditor(QWidget *parent) :
    DataSetEditor(parent),
    m_ui(new Ui::SqlDatasetEditor)
{
    m_ui->setupUi(this);

    SQLSyntaxHighlighter * syntax = new SQLSyntaxHighlighter(m_ui->textEdit->document());
}

SqlDatasetEditor::~SqlDatasetEditor()
{
    delete m_ui;
}

void SqlDatasetEditor::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SqlDatasetEditor::setDataset(Report::DataSet* dtst)
{
    Q_ASSERT(dtst);
    m_dataset = dtst;
    if (! dynamic_cast<SqlDataset*> (dtst))
	return;

    m_ui->textEdit->setPlainText(dynamic_cast<SqlDataset*> (dtst)->text());
}

void SqlDatasetEditor::sync()
{
    if (!dynamic_cast<SqlDataset*> (m_dataset))
	return;
    dynamic_cast<SqlDataset*> (m_dataset)->setText( m_ui->textEdit->toPlainText());
}

