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
#ifndef REPORTEXPORTINTERFACE_H
#define REPORTEXPORTINTERFACE_H
#include <QObject>
#include <QDomNode>

#include "globals.h"

namespace Report
{

class KONTAMABIL_EXPORTS ExportInterface: public QObject
{
	Q_OBJECT

public:
	ExportInterface(QObject *parent = 0);
	virtual ~ExportInterface();
	virtual QString exportFormat() = 0;
	virtual void execExport(QDomNode & exportNode) = 0;
};
}

Q_DECLARE_INTERFACE(Report::ExportInterface, "ro.bigendian.Report.ExportInterface/1.0");

#endif
