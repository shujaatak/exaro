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
#include <QTableWidget>
#include <QSplitter>

#include "designerscriptwidget.h"
#include "scriptsyntaxhighlighter.h"
#include "editor/scriptedit.h"
#include <message.h>

namespace Report
{

DesignerScriptWidget::DesignerScriptWidget(QWidget* parent)
		: QWidget(parent)
{
	gridLayout = new QGridLayout(this);
//	QSplitter *splitter = new QSplitter(Qt::Vertical, this);

	textEdit = new ScriptEdit( this );
	QToolBar * tb = new QToolBar(this);
//	varsWidget = new QTableWidget();
//	varsWidget->setColumnCount(2);
//	QStringList h;
//	h << tr("Variable") << tr ("Value");
//	varsWidget->setHorizontalHeaderLabels(h);

//	textEdit->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
//	varsWidget->setSizePolicy( QSizePolicy::Preferred ,QSizePolicy::Minimum)  ;

//	splitter->addWidget(textEdit);
//	splitter->addWidget(varsWidget);
//	splitter->setStretchFactor(0, 1);

	gridLayout->addWidget(tb, 0, 0, 1, 1);
	gridLayout->addWidget(textEdit, 1, 0, 1, 1);

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
	m_pasteAction->setEnabled(textEdit ->canPaste());

	connect(textEdit , SIGNAL(copyAvailable(bool)) , m_copyAction, SLOT(setEnabled(bool)));
	connect(textEdit , SIGNAL(copyAvailable(bool)) , m_cutAction, SLOT(setEnabled(bool)));

	connect(m_validateAction, SIGNAL(triggered()), this, SLOT(validate()));

	connect(m_copyAction, SIGNAL(triggered()), textEdit , SLOT(copy()));
	connect(m_cutAction, SIGNAL(triggered()), textEdit , SLOT(cut()));
	connect(m_pasteAction, SIGNAL(triggered()), textEdit , SLOT(paste()));

	connect(textEdit , SIGNAL(undoAvailable(bool)) , m_undoAction, SLOT(setEnabled(bool)));
	connect(textEdit , SIGNAL(redoAvailable(bool)) , m_redoAction, SLOT(setEnabled(bool)));

	connect(m_undoAction, SIGNAL(triggered()), textEdit , SLOT(undo()));
	connect(m_redoAction, SIGNAL(triggered()), textEdit , SLOT(redo()));

//	varsWidget->hide();
}

QString DesignerScriptWidget::text()
{
    return textEdit->toPlainText();
}

void DesignerScriptWidget::setText(const QString & string)
{
    textEdit->setPlainText(string);
}

bool DesignerScriptWidget::isValid()
{
    QScriptEngine se(this);
//     qDebug("Error = ", qPrintable(QScriptEngine::checkSyntax (textEdit->toPlainText()).errorMessage()));
//    qDebug("script \n %s", qPrintable(textEdit->toPlainText()));
    return se.canEvaluate(textEdit->toPlainText());
}

void DesignerScriptWidget::validate()
{
    if (isValid())
	Message::instance()->show(tr("the script it's ok"));
    else
	Message::instance()->show(tr("the script is invalid"));

    emit refreshVariables();
}

DesignerScriptWidget::~DesignerScriptWidget()
{
	delete m_syntax;
}

QStringList DesignerScriptWidget::variables()
{
    QStringList list;

    QRegExp rx("\\$(\\w{1,10})\\$");
    QString str = textEdit->toPlainText();
    int pos = 0;

    while ((pos = rx.indexIn(str, pos)) != -1) {
	if (!list.contains(rx.cap(1)) )
	    list.append(rx.cap(1));
	pos += rx.matchedLength();
    }

    return list;
}


/*
void DesignerScriptWidget::refreshVariables()
{
    QStringList list;

    QRegExp rx("\\$(\\w{1,10})\\$");
    QString str = textEdit->toPlainText();
    int pos = 0;

    while ((pos = rx.indexIn(str, pos)) != -1) {
	if (!list.contains(rx.cap(1)) )
	    list.append(rx.cap(1));
	pos += rx.matchedLength();
    }


    for (int i=0; i<varList.size(); i++)
	if (!list.contains(varList.at(i)))
	    emit removeVar(varList.at(i));

    for (int i=0; i<list.size(); i++)
	if (!varList.contains(list.at(i)))
	    emit addVar(list.at(i));

    varList = list;
}
*/

}
