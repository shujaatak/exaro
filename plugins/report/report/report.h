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

#ifndef REPORT_H
#define REPORT_H

#include "reportinterface.h"

class StdReport : public Report::ReportInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ReportInterface);
	Q_PROPERTY(int menuGroup READ menuGroup WRITE setMenuGroup)
	Q_PROPERTY(int order READ order WRITE setOrder)
	Q_PROPERTY(QStringList menus READ menus WRITE setMenus)
	Q_PROPERTY(QPixmap icon READ icon WRITE setIcon)
	Q_PROPERTY(QString description READ description WRITE setSescription)
	Q_PROPERTY(QString recommends READ recommends WRITE setRecommends)

public:
	StdReport(QObject *parent = 0);

	int order();
	void setOrder(int order);

	/**
	 * Return the menu how contain this report;
	 * @return menu
	 */
	QStringList menus();
	/**
	 * Sets the menu how will contain this report. The menu should be this form: /menu/menu1/menu2/menu3
	 * @param menu
	 */
	void setMenus(const QStringList &menu);

	QString description();
	void setSescription(const QString & description);

	QPixmap icon();
	void setIcon(const QPixmap & icon);

	QString recommends();
	void setRecommends(const QString &recommends);

	int menuGroup();
	void setMenuGroup(int menuGroup);

	Report::ReportInterface * createInstance(QObject * parent = 0);
	~StdReport();
private:
	int m_menuGroup;
	int m_order;
	QStringList m_menus;
	QPixmap m_icon;
	QString m_description;
	QString m_recommends;
};

#endif
