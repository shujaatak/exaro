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
#ifndef FAKEREPORTPAINTDEVICE_H
#define FAKEREPORTPAINTDEVICE_H

#include <QPaintDevice>
#include <QPrinter>
#include "fakepaintengine.h"

namespace Report
{


class FakePaintDevice : public QPaintDevice
{
private:
	friend class PaintEngine;

protected:
	int metric ( PaintDeviceMetric metric ) const;

public:
	FakePaintDevice();
	QPaintEngine * paintEngine() const;

private:
	FakePaintEngine * m_paintEngine;
	QPrinter m_printer;
};

}

#endif
