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
#ifndef REPORTPAGE_H
#define REPORTPAGE_H
#include <QPaintDevice>
#include <QDomNode>
#include <QPrinter>
#include <QTransform>

namespace Report
{

class Document;

class Page
{
public:
	enum SearchDirection {NextResult, PreviousResult };
public:
	~Page();
	void render(QPaintDevice * device);
	void render(QPainter * painter);
	QPrinter::PaperSize paperSize() const;
	QPrinter::Orientation paperOrientation() const;
	QSize pageSize() const;
	bool search(const QString &text, QRectF &rect, SearchDirection direction = NextResult, bool caseSensitive = false, int rectMargin = 5);

private:
	QTransform m_worldTransform;

	Page(QDomNode pageNode);
	friend class Document;

	inline void drawState(QPainter & p, QDomElement element);
	inline void drawRect(QPainter & p, QDomElement element);
	inline void drawLine(QPainter & p, QDomElement element);
	inline void drawEllipse(QPainter & p, QDomElement element);
	inline void drawPath(QPainter & p, QDomElement element);
	inline void drawPoint(QPainter & p, QDomElement element);
	inline void drawPolygon(QPainter & p, QDomElement element);
	inline void drawPixmap(QPainter & p, QDomElement element);
	inline void drawTextItem(QPainter & p, QDomElement element);
	inline void drawTiledPixmap(QPainter & p, QDomElement element);
	inline void drawImage(QPainter & p, QDomElement element);

private:
	QDomNode m_pageNode;
	QPrinter::PaperSize m_paperSize;
	QPrinter::Orientation m_paperOrientation;
	QDomElement m_searchNode;
	QString m_searchText;
};

}

#endif
