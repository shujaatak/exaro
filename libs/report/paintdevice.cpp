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
#include <QPrintEngine>

#include "paintdevice.h"
#include "paintengine.h"
#include "globals.h"

namespace Report
{

PaintDevice::PaintDevice()
		: QPaintDevice(), m_domDocument(0), m_docEnded(false), m_pageNumber(0)
{
	m_paintEngine = new PaintEngine();
	m_paintEngine->begin(this);
	m_paperSize = QPrinter::A4;
	m_paperOrientation = QPrinter::Portrait;
}


PaintDevice::~PaintDevice()
{
	delete m_paintEngine;
	if (!m_docEnded)
		endDoc();
}

QPaintEngine * PaintDevice::paintEngine() const
{
	return m_paintEngine;
}

void PaintDevice::setOutputDocument(QDomDocument * outputDocument)
{
	m_domDocument = outputDocument;
	m_reportRoot = m_domDocument->createElement("report");
	m_paintEngine->setDomDocument(m_domDocument);
	m_currentPage = m_domDocument->createElement("Page");
	m_paintEngine->setPageNode(m_currentPage);
}

void PaintDevice::setReportRoot(QDomNode & reportRoot)
{
	m_reportRoot = reportRoot;
	m_currentPage = m_domDocument->createElement("Page");
	m_paintEngine->setPageNode(m_currentPage);
}


QDomNode & PaintDevice::pageNode()
{
	return m_currentPage;
}

QDomDocument * PaintDevice::domDocument()
{
	return m_domDocument;
}

void PaintDevice::setPaperSize(QPrinter::PaperSize size)
{
	m_paperSize = size;
}

void PaintDevice::setPaperOrientation(QPrinter::Orientation orientation)
{
	m_paperOrientation = orientation;
}

void PaintDevice::newPage()
{
	if (m_currentPage.isNull() || m_currentPage.toElement().isNull() || !m_currentPage.toElement().text().length())
		return; // the page is empty ?!?!?

	m_currentPage.appendChild(variatToDom(m_domDocument, "pageSize", m_paperSize));
	m_currentPage.appendChild(variatToDom(m_domDocument, "pageOrientation", m_paperOrientation));
	m_reportRoot.appendChild(m_currentPage);
	m_currentPage = m_domDocument->createElement("Page");
	m_paintEngine->setPageNode(m_currentPage);
}

void PaintDevice::endDoc()
{
	if (m_domDocument && !m_currentPage.isNull() && !m_currentPage.toElement().isNull() && m_currentPage.toElement().text().length())
	{
		m_currentPage.appendChild(variatToDom(m_domDocument, "pageSize", m_paperSize));
		m_currentPage.appendChild(variatToDom(m_domDocument, "pageOrientation", m_paperOrientation));
		m_reportRoot.appendChild(m_currentPage);
	}
	if (m_domDocument)
		m_domDocument->appendChild(m_reportRoot);
}

int PaintDevice::metric(QPaintDevice::PaintDeviceMetric metric) const
{
	QPrinter p;
	return p.printEngine()->metric(metric);
}

}
