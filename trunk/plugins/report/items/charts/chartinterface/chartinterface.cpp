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
#include <sqlquery.h>
#include <QSqlRecord>
#include <QSqlField>

#include "chartinterface.h"
#include "chartcontainer.h"

namespace Report {

ChartInterface::ChartInterface(QGraphicsItem* parent, QObject* parentObject): ItemInterface(parent, parentObject)
{
}

ChartInterface::~ChartInterface()
{
}

QList<ChartInterface::_chartValue> ChartInterface::values()
{
	QList<_chartValue> vl;
	if (dynamic_cast<ChartDataCollector*>(parent()))
		vl=dynamic_cast<ChartDataCollector*>(parent())->values();
	return vl;
}
}

