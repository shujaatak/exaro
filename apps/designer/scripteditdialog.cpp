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
#include <QScriptEngine>
#include <QMessageBox>

#include "scripteditdialog.h"
#include "scriptsyntaxhighlighter.h"

ScriptEditDialog::ScriptEditDialog(QWidget* parent, Qt::WFlags fl)
		: QDialog(parent, fl)
{
	setWindowTitle(tr("Script editor"));
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
	m_validateAction = tb->addAction(QIcon(":/images/validate.png"), tr("Validate"));

	m_copyAction->setEnabled(false);
	m_cutAction->setEnabled(false);
	m_undoAction->setEnabled(false);
	m_redoAction->setEnabled(false);
	m_pasteAction->setEnabled(plainTextEdit->canPaste());

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	connect(plainTextEdit, SIGNAL(copyAvailable(bool)) , m_copyAction, SLOT(setEnabled(bool)));
	connect(plainTextEdit, SIGNAL(copyAvailable(bool)) , m_cutAction, SLOT(setEnabled(bool)));

	connect(m_validateAction, SIGNAL(triggered()), this, SLOT(validate()));

	connect(m_copyAction, SIGNAL(triggered()), plainTextEdit, SLOT(copy()));
	connect(m_cutAction, SIGNAL(triggered()), plainTextEdit, SLOT(cut()));
	connect(m_pasteAction, SIGNAL(triggered()), plainTextEdit, SLOT(paste()));

	connect(plainTextEdit, SIGNAL(undoAvailable(bool)) , m_undoAction, SLOT(setEnabled(bool)));
	connect(plainTextEdit, SIGNAL(redoAvailable(bool)) , m_redoAction, SLOT(setEnabled(bool)));

	connect(m_undoAction, SIGNAL(triggered()), plainTextEdit, SLOT(undo()));
	connect(m_redoAction, SIGNAL(triggered()), plainTextEdit, SLOT(redo()));
	m_syntax = new ScriptSyntaxHighlighter(plainTextEdit->document());

}

QString ScriptEditDialog::text()
{
	return plainTextEdit->toPlainText();
}

void ScriptEditDialog::setText(const QString & string)
{
	plainTextEdit->setPlainText(string);
}

bool ScriptEditDialog::isValid()
{
	QScriptEngine se(this);
	return se.canEvaluate(plainTextEdit->toPlainText());
}

void ScriptEditDialog::accept()
{
	if (isValid())
		QDialog::accept();
	else
		QMessageBox::critical(this, tr("Error"), tr("the script is invalid"), QMessageBox::Ok);
}

void ScriptEditDialog::validate()
{
	if (isValid())
		QMessageBox::information(this, tr("Ok"), tr("the script is ok"), QMessageBox::Ok);
	else
		QMessageBox::critical(this, tr("Error"), tr("the script is invalid"), QMessageBox::Ok);

}

ScriptEditDialog::~ScriptEditDialog()
{
	delete m_syntax;
}


