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
#include <QPainter>
#include <QVariant>
#include <QPixmap>
#include <QImage>
#include <QDomNodeList>
#include <QDebug>

#include "page.h"
#include "globals.h"

namespace Report
{

Page::Page(QDomNode pageNode)
{
	m_pageNode = pageNode;
	m_paperSize = (QPrinter::PaperSize)domToVariant(m_pageNode.firstChildElement("pageSize")).toInt();
	m_paperOrientation = (QPrinter::Orientation)domToVariant(m_pageNode.firstChildElement("pageOrientation")).toInt();;
}

Page::~Page()
{
}

void Page::drawState(QPainter & p, QDomElement element)
{
	Qt::ClipOperation m_clipOperation = Qt::NoClip;

	QDomNodeList elements = element.childNodes();
	for (int i = 0;i < elements.size();i++)
	{
		if (elements.at(i).toElement().tagName() == "transform")
		{
			p.setWorldTransform(m_worldTransform, false);
			p.setWorldTransform(domToVariant(elements.at(i).toElement()).value<QTransform>(), true);
			continue;
		}

		if (elements.at(i).toElement().tagName() == "backgroundBrush")
		{
			p.setBackground(domToVariant(elements.at(i).toElement()).value<QBrush>());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "backgroundMode")
		{
			p.setBackgroundMode((Qt::BGMode)domToVariant(elements.at(i).toElement()).toInt());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "brush")
		{
			p.setBrush(domToVariant(elements.at(i).toElement()).value<QBrush>());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "brushOrigin")
		{
			p.setBrushOrigin(domToVariant(elements.at(i).toElement()).toPointF());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "compositionMode")
		{
			p.setCompositionMode((QPainter::CompositionMode)domToVariant(elements.at(i).toElement()).toInt());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "font")
		{
			p.setFont(domToVariant(elements.at(i).toElement()).value<QFont>());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "opacity")
		{
			p.setOpacity(domToVariant(elements.at(i).toElement()).toDouble());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "pen")
		{
			p.setPen(domToVariant(elements.at(i).toElement()).value<QPen>());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "renderHints")
		{
			p.setRenderHints((QPainter::RenderHints)domToVariant(elements.at(i).toElement()).toInt());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "isClipEnabled")
		{
			p.setClipping(domToVariant(elements.at(i).toElement()).toBool());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "clipOperation")
		{
			m_clipOperation = (Qt::ClipOperation)domToVariant(elements.at(i).toElement()).toInt();
			continue;
		}

		if (elements.at(i).toElement().tagName() == "clipRegion")
		{
			p.setClipRegion(domToVariant(elements.at(i).toElement()).value<QRegion>(), m_clipOperation);
			continue;
		}

		if (elements.at(i).toElement().tagName() == "clipPath")
		{
			p.setClipPath(domToVariant(elements.at(i).toElement()).value<QPainterPath>(), m_clipOperation);
			continue;
		}


	}
}

void Page::drawRect(QPainter & p, QDomElement element)
{
	p.drawRect(domToVariant(element).toRectF());
}

void Page::drawLine(QPainter & p, QDomElement element)
{
	p.drawLine(domToVariant(element).toLineF());
}

void Page::drawEllipse(QPainter & p, QDomElement element)
{
	p.drawEllipse(domToVariant(element).toRectF());
}

void Page::drawPath(QPainter & p, QDomElement element)
{
	p.drawPath(domToVariant(element).value<QPainterPath>());
}

void Page::drawPoint(QPainter & p, QDomElement element)
{
	p.drawPoint(domToVariant(element).toPointF());
}

void Page::drawPolygon(QPainter & p, QDomElement element)
{
	Q_UNUSED(p);
	Q_UNUSED(element);
	qWarning() << "drawPolygon is not finished";
}

void Page::drawPixmap(QPainter & p, QDomElement element)
{
	p.drawPixmap(domToVariant(element.firstChildElement("toRect")).toRectF(), domToVariant(element.firstChildElement("img")).value<QPixmap>(), domToVariant(element.firstChildElement("sourceRect")).toRectF());
}

void Page::drawTextItem(QPainter & p, QDomElement element)
{
	p.setFont(domToVariant(element.firstChildElement("font")).value<QFont>());
#ifndef WIN32
#warning "FIXME: I don't have any idea how to use this propieties"
#endif
	/*
		// I don't have any idea how to use this propieties

		domToVariant(element.firstChildElement("renderFlags"));
		domToVariant(element.firstChildElement("width"));
		domToVariant(element.firstChildElement("ascent"));
		domToVariant(element.firstChildElement("descent"));
	*/
	p.drawText(domToVariant(element.firstChildElement("toPoint")).toPointF(), domToVariant(element.firstChildElement("string")).toString());
}

void Page::drawTiledPixmap(QPainter & p, QDomElement element)
{
	p.drawTiledPixmap(domToVariant(element.firstChildElement("toRect")).toRectF(), domToVariant(element.firstChildElement("img")).value<QPixmap>(), domToVariant(element.firstChildElement("toPoint")).toPointF());
}

void Page::drawImage(QPainter & p, QDomElement element)
{
	p.drawImage(domToVariant(element.firstChildElement("toRect")).toRectF(), (domToVariant(element.firstChildElement("img")).value<QPixmap>()).toImage(), domToVariant(element.firstChildElement("sourceRect")).toRectF(), (Qt::ImageConversionFlags) domToVariant(element.firstChildElement("flags")).toInt());
}


void Page::render(QPaintDevice * device)
{
	QPainter * p=new QPainter(device);
	render(p);
	p->end();
	delete p;
}

void Page::render(QPainter * p)
{
	m_worldTransform=p->worldTransform();
	QDomNodeList elements = m_pageNode.childNodes();
	for (int i = 0;i < elements.size();i++)
	{

		if (elements.at(i).toElement().tagName() == "stateChanged")
		{
			drawState(*p, elements.at(i).toElement());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "image")
		{
			drawImage(*p, elements.at(i).toElement());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "tiledPixmap")
		{
			drawTiledPixmap(*p, elements.at(i).toElement());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "text")
		{
			drawTextItem(*p, elements.at(i).toElement());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "pixmap")
		{
			drawPixmap(*p, elements.at(i).toElement());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "polygon")
		{
			drawPolygon(*p, elements.at(i).toElement());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "point")
		{
			drawPoint(*p, elements.at(i).toElement());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "path")
		{
			drawPath(*p, elements.at(i).toElement());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "ellipse")
		{
			drawEllipse(*p, elements.at(i).toElement());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "line")
		{
			drawLine(*p, elements.at(i).toElement());
			continue;
		}

		if (elements.at(i).toElement().tagName() == "rect")
			drawRect(*p, elements.at(i).toElement());

	}
}

QPrinter::PaperSize Page::paperSize() const
{
	return m_paperSize;
}

QSize Page::pageSize() const
{
	QPrinter p;
	p.setPaperSize(m_paperSize);
	p.setOrientation(m_paperOrientation);
	QSizeF sz=p.paperSize(QPrinter::Millimeter);
	sz/=UNIT;
	return sz.toSize();
}

bool Page::search(const QString &text, QRectF &rect, SearchDirection direction, bool caseSensitive, int rectMargin)
{
	if (!text.length())
	{
		m_searchText = "";
		return false;
	}
	if ((caseSensitive && m_searchText != text) || (!caseSensitive && m_searchText.toLower() != text.toLower()))
	{
		if (m_searchNode.isNull() && m_searchText.length())
		{
			m_searchText = "";
			return false;
		}
		if (direction == NextResult)
			m_searchNode = m_pageNode.firstChildElement("text");
		else
			m_searchNode = m_pageNode.lastChildElement("text");
	}

	m_searchText = text;

	while (!m_searchNode.isNull())
	{
		QString nodeString = m_searchNode.firstChildElement("string").text();
		if (nodeString.contains(m_searchText, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive))
		{
			QPointF pos = domToVariant(m_searchNode.firstChildElement("toPoint")).toPointF();
			QFontMetrics f(domToVariant(m_searchNode.firstChildElement("font")).value<QFont>());

			pos.setX(pos.x() + f.width(nodeString.left(nodeString.indexOf(m_searchText, 0, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive))) - rectMargin);
			pos.setY(pos.y() - rectMargin);
			rect = f.boundingRect(nodeString.mid(nodeString.indexOf(m_searchText, 0, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive), m_searchText.length()));
			rect.translate(pos);

			QDomElement transformElement = m_searchNode.previousSiblingElement("stateChanged");
			while (!transformElement.isNull())
			{
				if (!transformElement.firstChildElement("transform").isNull())
				{
					QTransform tr = domToVariant(transformElement.firstChildElement("transform")).value<QTransform>();
					rect.translate(tr.dx(), tr.dy());
					break;
				}
				transformElement = transformElement.previousSiblingElement("stateChanged");
			}
			rect.setWidth(rect.width() + rectMargin*2);
			rect.setHeight(rect.height() + rectMargin*2);

			if (direction == NextResult)
				m_searchNode = m_searchNode.nextSiblingElement("text");
			else
				m_searchNode = m_searchNode.previousSiblingElement("text");
			return true;
		}

		if (direction == NextResult)
			m_searchNode = m_searchNode.nextSiblingElement("text");
		else
			m_searchNode = m_searchNode.previousSiblingElement("text");
	};
	m_searchText = "";
	return false;
}
}
