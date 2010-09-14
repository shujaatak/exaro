/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
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

#include <QAction>
#include <QApplication>

#include "searchwidget.h"

namespace Report
{

SearchWidget::SearchWidget(QWidget *parent)
		: QToolBar(parent), m_searchNext(true), minCharSearch(3)
{
	QAction * act = addAction(QIcon(":/images/quit.png"), tr("&Close"));
	act->setShortcut(QKeySequence(Qt::Key_Escape));
	connect(act, SIGNAL(triggered(bool)), SLOT(hide()));

	addSeparator();

	act = addAction(QIcon(":/images/search-up.png"), tr("&Previous"));
	act->setShortcut(QKeySequence(QKeySequence::FindPrevious));
	connect(act, SIGNAL(triggered(bool)), SLOT(searchPrevious()));

	act = addAction(QIcon(":/images/search-down.png"), tr("&Next"));
	act->setShortcut(QKeySequence(QKeySequence::FindNext));
	connect(act, SIGNAL(triggered(bool)), SLOT(searchNext()));

	m_lineEdit = new QLineEdit(this);
	addWidget(m_lineEdit);
	connect(m_lineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
	connect(m_lineEdit, SIGNAL(returnPressed()), SLOT(textChanged()));

	QPalette pal = QApplication::palette();
	m_lineEdit->setPalette(pal);
}

void SearchWidget::searchNext()
{
	QPalette pal = QApplication::palette();
	m_lineEdit->setPalette(pal);
	m_searchNext = true;
	emit(searchNext(m_lineEdit->text()));
}

void SearchWidget::searchPrevious()
{
	QPalette pal = QApplication::palette();
	m_lineEdit->setPalette(pal);
	m_searchNext = false;
	emit(searchPrevious(m_lineEdit->text()));
}

void SearchWidget::textChanged()
{
	QPalette pal = QApplication::palette();
	m_lineEdit->setPalette(pal);

	if (m_searchNext)
		emit(searchNext(m_lineEdit->text()));
	else
		emit(searchPrevious(m_lineEdit->text()));
}

void SearchWidget::textChanged(const QString& text)
{
	QPalette pal = QApplication::palette();
	m_lineEdit->setPalette(pal);

	if (text.length() >= minCharSearch)
	{
		if (m_searchNext)
			emit(searchNext(text));
		else
			emit(searchPrevious(text));
	}
}

void SearchWidget::setVisible(bool visible)
{
	QToolBar::setVisible(visible);
	if (visible)
	{
		m_lineEdit->selectAll();
		m_lineEdit->setFocus();
	}
	else
		emit(closed());
}

void SearchWidget::notFound()
{
	QPalette pal = QApplication::palette();
	pal.setColor(QPalette::Base, Qt::red);
	pal.setColor(QPalette::Text, Qt::white);
	m_lineEdit->setPalette(pal);
}

SearchWidget::~SearchWidget()
{
}

}
