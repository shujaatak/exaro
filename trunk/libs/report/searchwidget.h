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
#ifndef REPORTSEARCHWIDGET_H
#define REPORTSEARCHWIDGET_H

#include <QToolBar>
#include <QLineEdit>

namespace Report
{

class SearchWidget : public QToolBar
{
	Q_OBJECT

public:
	SearchWidget(QWidget *parent = 0);
	~SearchWidget();
	void setVisible(bool visible);

private:
	QLineEdit * m_lineEdit;
	bool m_searchNext;
	int minCharSearch;

public slots:
	void notFound();
	void searchNext();
	void searchPrevious();
	void textChanged(const QString&);
	void textChanged();

signals:
	void searchNext(const QString & text);
	void searchPrevious(const QString & text);
	void closed();
};

}

#endif
