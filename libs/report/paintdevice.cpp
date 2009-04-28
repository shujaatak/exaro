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

#include <QPrintEngine>

#include "paintdevice.h"
#include "paintengine.h"
#include "globals.h"

namespace Report
{

PaintDevice::PaintDevice(QIODevice * doc)
		: QPaintDevice(), m_doc(doc),m_allowEmptyPages(false),m_pagePos(0)
{
	m_printer.setOutputFormat(QPrinter::PdfFormat);
	m_printer.setOrientation(QPrinter::Portrait);
	m_printer.setPaperSize(QPrinter::A4);

	m_paintEngine = new PaintEngine(m_doc);
	m_paintEngine->begin(this);
	m_currentPageStruct.size=0;
	m_currentPageStruct.height=m_printer.heightMM()*10;
	m_currentPageStruct.width=m_printer.widthMM()*10;
	m_currentPageStruct.orientation=QPrinter::Portrait;
	m_doc->write((char*)&m_currentPageStruct, sizeof(m_currentPageStruct));
}


PaintDevice::~PaintDevice()
{
	qint64 pos=m_doc->pos();
	m_currentPageStruct.size=pos-m_pagePos;
	m_doc->seek(m_pagePos);
	m_doc->write((char*)&m_currentPageStruct, sizeof(m_currentPageStruct));
	delete m_paintEngine;
}

QPaintEngine * PaintDevice::paintEngine() const
{
	return m_paintEngine;
}


void PaintDevice::setPaperSize(const QSizeF & pageSize)
{
	m_currentPageStruct.height=pageSize.height();
	m_currentPageStruct.width=pageSize.width();
}

void PaintDevice::setPaperOrientation(QPrinter::Orientation orientation)
{
	m_currentPageStruct.orientation=orientation;
}

void PaintDevice::newPage()
{
	qint64 pos=m_doc->pos();
	m_currentPageStruct.size=pos-m_pagePos;
	if (!m_allowEmptyPages && m_currentPageStruct.size<=sizeof(m_currentPageStruct))
		return; // the page is empty 
	m_doc->seek(m_pagePos);
	m_doc->write((char*)&m_currentPageStruct, sizeof(m_currentPageStruct));
	m_doc->seek(pos);
	m_pagePos=pos;
	m_currentPageStruct.size=0;
	m_doc->write((char*)&m_currentPageStruct, sizeof(m_currentPageStruct));
}

int PaintDevice::metric(QPaintDevice::PaintDeviceMetric metric) const
{
	return m_printer.printEngine()->metric(metric);
}

void PaintDevice::setPrinterName(const QString & name)
{
	m_printer.setPrinterName(name);
}
}
