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
#ifndef REPORTPAINTDEVICE_H
#define REPORTPAINTDEVICE_H

#include <QPaintDevice>
#include <QDomDocument>
#include <QPrinter>

namespace Report
{

class PaintEngine;

class PaintDevice : public QPaintDevice
{
private:
	QDomNode & pageNode();
	QDomDocument * domDocument();

	void endDoc();
	friend class PaintEngine;

public:
	PaintDevice();

	~PaintDevice();
	void setOutputDocument(QDomDocument * outputDocument);
	void setReportRoot(QDomNode & reportRoot);
	void setPaperSize(QPrinter::PaperSize size);
	void setPaperOrientation(QPrinter::Orientation orientation);
	void newPage();
	QPaintEngine * paintEngine() const;

	int metric(QPaintDevice::PaintDeviceMetric metric) const;

private:
	PaintEngine * m_paintEngine;
	QDomDocument * m_domDocument;
	QDomNode m_reportRoot;
	QDomNode m_currentPage;
	QPrinter::PaperSize m_paperSize;
	QPrinter::Orientation m_paperOrientation;
	bool m_docEnded;
	int m_pageNumber;
};

}

#endif
