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

Document::Document(QIODevice * doc)
{
	m_doc=doc;
	PaintDevice::PageStruct ps;
	qint64 pos=0;
	doc->seek(0);
	while(!doc->atEnd())
	{
		doc->read((char*)&ps, sizeof(ps));
		m_pages.push_back(pos);
		pos+=ps.size;
		doc->seek(pos);
	}
}

int Document::numPages()
{
	return m_pages.size();
}

Page * Document::page(int pageNumber)
{
	if (pageNumber >= m_pages.size())
		return 0;
	return new Page(m_doc, m_pages[pageNumber]);
}

}
