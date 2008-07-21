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
#include "document.h"
#include "page.h"

namespace Report
{


Document::Document(QDomDocument * doc): m_numPages(0)
{
	if (doc)
		setReportDocument(doc);
}

Document::Document(QDomNode & node): m_numPages(0)
{
	setReportNode(node);
}

void Document::calcPages()
{
	m_numPages = m_docNode.childNodes().size();
}

void Document::setReportDocument(QDomDocument * doc)
{
	m_docNode = doc->documentElement();
	calcPages();
}

void Document::setReportNode(QDomNode & node)
{
	m_docNode = node;
	calcPages();
}

int Document::numPages()
{
	return m_numPages;
}

Page * Document::page(int pageNumber)
{
	if (pageNumber >= m_numPages)
		return 0;
	return new Page(m_docNode.childNodes().at(pageNumber));
}

}
