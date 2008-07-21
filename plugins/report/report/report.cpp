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

#include <QtCore>
#include "report.h"

StdReport::StdReport(QObject *parent)
		: Report::ReportInterface(parent),m_menuGroup(90),m_order(90),m_menus(QStringList()<<"/Program/Reports")
{
}


StdReport::~StdReport()
{
}

int StdReport::order()
{
	return m_order;
}

void StdReport::setOrder(int order)
{
	m_order=order;
}

QStringList StdReport::menus()
{
	return m_menus;
}

void StdReport::setMenus(const QStringList &menus)
{
	m_menus = menus;
}

QString StdReport::description()
{
	return m_description;
}

void StdReport::setSescription(const QString & description)
{
	m_description=description;
}

QPixmap StdReport::icon()
{
	return m_icon;
}

void StdReport::setIcon(const QPixmap & icon)
{
	m_icon=icon;
}


QString StdReport::recommends()
{
	return m_recommends;
}
void StdReport::setRecommends(const QString &recommends)
{
	m_recommends=recommends;
}

int StdReport::menuGroup()
{
	return m_menuGroup;
}
void StdReport::setMenuGroup(int menuGroup)
{
	m_menuGroup=menuGroup;
}


Report::ReportInterface * StdReport::createInstance(QObject * parent)
{
	return new StdReport(parent);
}

Q_EXPORT_PLUGIN2(report, StdReport)
