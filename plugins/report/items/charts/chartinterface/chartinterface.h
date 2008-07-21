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
#ifndef CHARTINTERFACE_H
#define CHARTINTERFACE_H

#include <QObject>
#include <QColor>
#include <iteminterface.h>

namespace Report
{

class ChartInterface : public Report::ItemInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);
public:
	struct _chartValue
	{
		QString key;
		qreal value;
		QColor color;
	};

public:
	ChartInterface(QGraphicsItem* parent = 0, QObject* parentObject = 0);
	~ChartInterface();
	/**
	 * Returns chart values
	 * @return chart values
	 */
	QList<_chartValue> values();
};

}

Q_DECLARE_INTERFACE(Report::ChartInterface, "ro.bigendian.Report.ChartInterface/1.0");

#endif
