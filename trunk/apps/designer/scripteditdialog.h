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
#ifndef SCRIPTEDITDIALOG_H
#define SCRIPTEDITDIALOG_H

#include <QDialog>
#include <QAction>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpacerItem>

class ScriptSyntaxHighlighter;

class ScriptEditDialog : public QDialog
{
	Q_OBJECT
public:
	ScriptEditDialog(QWidget* parent = 0, Qt::WFlags fl = 0);

	~ScriptEditDialog();

	QString text();
	void setText(const QString & string);

	bool isValid();

protected:
	void accept();

private slots:
	void validate();

private:
	QGridLayout *gridLayout;
	QPlainTextEdit *plainTextEdit;
	QHBoxLayout *horizontalLayout;
	QSpacerItem *horizontalSpacer;
	QPushButton *okButton;
	QPushButton *cancelButton;

	QAction * m_validateAction;

	QAction * m_undoAction;
	QAction * m_redoAction;

	QAction * m_copyAction;
	QAction * m_pasteAction;
	QAction * m_cutAction;
	ScriptSyntaxHighlighter * m_syntax;
};

#endif
