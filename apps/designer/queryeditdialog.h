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

#ifndef QUERYEDITDIALOG_H
#define QUERYEDITDIALOG_H

#include <QDialog>
#include <QAction>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpacerItem>

class SQLSyntaxHighlighter;

class QueryEditDialog : public QDialog
{
	Q_OBJECT

public:
	QueryEditDialog(QWidget* parent = 0, Qt::WFlags fl = 0);
	~QueryEditDialog();

	void setQuery(const QString & query);
	QString query();

private slots:
	void showResults();

private:
	QGridLayout *gridLayout;
	QPlainTextEdit *plainTextEdit;
	QHBoxLayout *horizontalLayout;
	QSpacerItem *horizontalSpacer;
	QPushButton *okButton;
	QPushButton *cancelButton;

	QAction * m_showResults;

	QAction * m_undoAction;
	QAction * m_redoAction;

	QAction * m_copyAction;
	QAction * m_pasteAction;
	QAction * m_cutAction;
	SQLSyntaxHighlighter * m_syntax;
};

#endif

