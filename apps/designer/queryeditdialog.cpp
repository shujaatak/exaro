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

#include <QToolBar>
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlError>
#include <QMessageBox>

#include "queryeditdialog.h"
#include "sqlsyntaxhighlighter.h"

QueryEditDialog::QueryEditDialog(QWidget* parent, Qt::WFlags fl)
		: QDialog(parent, fl)
{
	setWindowTitle(tr("Query editor"));
	resize(602, 399);
	gridLayout = new QGridLayout(this);
	plainTextEdit = new QPlainTextEdit(this);
	QToolBar * tb = new QToolBar(this);
	gridLayout->addWidget(tb, 0, 0, 1, 1);
	gridLayout->addWidget(plainTextEdit, 1, 0, 1, 1);
	horizontalLayout = new QHBoxLayout();
	horizontalSpacer = new QSpacerItem(178, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalLayout->addItem(horizontalSpacer);
	okButton = new QPushButton(this);
	horizontalLayout->addWidget(okButton);
	cancelButton = new QPushButton(this);
	horizontalLayout->addWidget(cancelButton);
	okButton->setText(tr("&OK"));
	cancelButton->setText(tr("&Cancel"));
	gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);
	QMetaObject::connectSlotsByName(this);

	m_copyAction = tb->addAction(QIcon(":/images/editcopy.png"), tr("Copy"));
	m_cutAction = tb->addAction(QIcon(":/images/editcut.png"), tr("Cut"));
	m_pasteAction = tb->addAction(QIcon(":/images/editpaste.png"), tr("Paste"));
	tb->addSeparator();
	m_undoAction = tb->addAction(QIcon(":/images/editundo.png"), tr("Undo"));
	m_redoAction = tb->addAction(QIcon(":/images/editredo.png"), tr("Redo"));
	tb->addSeparator();
	m_showResults = tb->addAction(QIcon(":/images/zoom-original.png"), tr("Test query"));

	m_copyAction->setEnabled(false);
	m_cutAction->setEnabled(false);
	m_undoAction->setEnabled(false);
	m_redoAction->setEnabled(false);
	m_pasteAction->setEnabled(plainTextEdit->canPaste());

	connect(okButton, SIGNAL(clicked()), SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

	connect(plainTextEdit, SIGNAL(copyAvailable(bool)) , m_copyAction, SLOT(setEnabled(bool)));
	connect(plainTextEdit, SIGNAL(copyAvailable(bool)) , m_cutAction, SLOT(setEnabled(bool)));

	connect(m_copyAction, SIGNAL(triggered()), plainTextEdit, SLOT(copy()));
	connect(m_cutAction, SIGNAL(triggered()), plainTextEdit, SLOT(cut()));
	connect(m_pasteAction, SIGNAL(triggered()), plainTextEdit, SLOT(paste()));

	connect(plainTextEdit, SIGNAL(undoAvailable(bool)) , m_undoAction, SLOT(setEnabled(bool)));
	connect(plainTextEdit, SIGNAL(redoAvailable(bool)) , m_redoAction, SLOT(setEnabled(bool)));

	connect(m_undoAction, SIGNAL(triggered()), plainTextEdit, SLOT(undo()));
	connect(m_redoAction, SIGNAL(triggered()), plainTextEdit, SLOT(redo()));

	connect(m_showResults, SIGNAL(triggered()), SLOT(showResults()));

	m_syntax = new SQLSyntaxHighlighter(plainTextEdit->document());
}


QueryEditDialog::~QueryEditDialog()
{
	delete m_syntax;
}

void QueryEditDialog::showResults()
{
	QSqlQueryModel *model = new QSqlQueryModel;
	model->setQuery(plainTextEdit->toPlainText());
	if (model->lastError().type()!=QSqlError::NoError)
	{
		QMessageBox::critical(this,tr("Can't exec query"), model->lastError().text());
		delete model;
		return;
	}
	QTableView *view = new QTableView(this);
	view->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
	view->setModel(model);
	view->show();
}

void QueryEditDialog::setQuery(const QString & query)
{
	plainTextEdit->setPlainText(query);
}

QString QueryEditDialog::query()
{
	return plainTextEdit->toPlainText();
}

