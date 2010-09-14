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

#include <QPainter>
#include <QVariant>
#include <QPixmap>
#include <QImage>
#include <QPaintEngine>
#include <QDebug>

#include "page.h"
#include "fakepaintdevice.h"
#include "globals.h"

namespace Report
{

Page::Page(QIODevice *doc, qint64 pos)
{
	m_doc=doc;
	doc->seek(pos);
	doc->read((char*)&m_pageStruct, sizeof(m_pageStruct));
	m_pos=pos+sizeof(m_pageStruct);
	m_toPos=pos+m_pageStruct.size;
	m_searchPos=m_pos;
}

Page::~Page()
{
}

void Page::drawState(QPainter * p)
{
	int state=readInt();
	if (state & QPaintEngine::DirtyHints)
		p->setRenderHints((QPainter::RenderHints)readInt());

	if (state & QPaintEngine::DirtyTransform)
	{
		qreal h11=readDouble();
		qreal h12=readDouble();
		qreal h13=readDouble();
		qreal h21=readDouble();
		qreal h22=readDouble();
		qreal h23=readDouble();
		qreal h31=readDouble();
		qreal h32=readDouble();
		qreal h33=readDouble();
		p->setWorldTransform(m_worldTransform, false);
		p->setWorldTransform(QTransform(h11, h12, h13, h21, h22, h23, h31, h32, h33),true);
	}

	if (state & QPaintEngine::DirtyBackground)
		p->setBackground(readBrush());

	if (state & QPaintEngine::DirtyBackgroundMode)
		p->setBackgroundMode((Qt::BGMode)readInt());

	if (state & QPaintEngine::DirtyBrush)
		p->setBrush(readBrush());

	if (state & QPaintEngine::DirtyBrushOrigin)
		p->setBrushOrigin(readPoint());

	if (state & QPaintEngine::DirtyCompositionMode)
		p->setCompositionMode((QPainter::CompositionMode)readInt());

	if (state & QPaintEngine::DirtyFont)
		p->setFont(readFont());

	if (state & QPaintEngine::DirtyOpacity)
		p->setOpacity(readDouble());

	if (state & QPaintEngine::DirtyPen)
		p->setPen(readPen());

	if (state & QPaintEngine::DirtyClipRegion)
	{
		QRegion r=readRegion();
		int co=readInt();
		p->setClipRegion(r,(Qt::ClipOperation)co);
	}
	if (state & QPaintEngine::DirtyClipPath)
	{
		QPainterPath pp=readPath();
		int co=readInt();
		p->setClipPath(pp,(Qt::ClipOperation)co);
	}
	if (state & QPaintEngine::DirtyClipEnabled)
		p->setClipping(readInt());
}

void Page::drawRect(QPainter * p)
{
	int rects=readInt();
	while(rects)
	{
		QRectF rc=readRect();
#ifdef __GNUC__
#warning I dont know way but this is not working 
#else
#pragma WARNING( I dont know way but this is not working  )
#endif
//		if (!m_exposeRect.intersects(rc))
		if(!m_search)
			p->drawRect(rc);
		rects--;
	}
}

void Page::drawLine(QPainter * p)
{
	int lines=readInt();
	while(lines)
	{
		QPointF p1=readPoint();
		QPointF p2=readPoint();
#ifdef __GNUC__
#warning I dont know way but this is not working 
#else
#pragma WARNING( I dont know way but this is not working  )
#endif
//		if (!m_exposeRect.intersects(QRectF(p1,p2)))
		if(!m_search)
			p->drawLine(p1,p2);
		lines--;
	}
}

void Page::drawEllipse(QPainter * p)
{
	QRectF rc=readRect();
#ifdef __GNUC__
#warning I dont know way but this is not working 
#else
#pragma WARNING( I dont know way but this is not working  )
#endif
//	if (m_exposeRect.intersects(rc))
		if(!m_search)
			p->drawEllipse(rc);
}

void Page::drawPath(QPainter * p)
{
	QPainterPath pt=readPath();
#ifdef __GNUC__
#warning I dont know way but this is not working 
#else
#pragma WARNING( I dont know way but this is not working  )
#endif
//	if(pt.intersects(m_exposeRect))
		if(!m_search)
			p->drawPath(pt);
}

void Page::drawPoint(QPainter * p)
{
	if(!m_search)
		p->drawPoint(readPoint());
}

void Page::drawPixmap(QPainter * p)
{
	QRectF r,sr;
	r=readRect();
	sr=readRect();
	QPixmap px=QPixmap::fromImage(readImage());
#ifdef __GNUC__
#warning I dont know way but this is not working 
#else
#pragma WARNING( I dont know way but this is not working  )
#endif
//	if (m_exposeRect.intersects(r))
	if(!m_search)
		p->drawPixmap(r, px, sr);
}

void Page::drawTextItem(QPainter * p)
{
	QPointF pt=readPoint();
	p->setFont(readFont());
	int renderFlags;
	double width, ascent,descent;
	renderFlags=readInt();
	width=readDouble();
	ascent=readDouble();
	descent=readDouble();
	QString st=readString();
#ifdef __GNUC__
#warning I dont know way but this is not working 
#else
#pragma WARNING( I dont know way but this is not working  )
#endif
//	if (QRectF(pt.x(), pt.y(), width, ascent+descent).intersects(m_exposeRect)) 
	p->drawText(pt, st);
}

void Page::drawTiledPixmap(QPainter * p)
{
	QRectF rt=readRect();
	QPointF pt=readPoint();
	QPixmap px=QPixmap::fromImage(readImage());
#ifdef __GNUC__
#warning I dont know way but this is not working 
#else
#pragma WARNING( I dont know way but this is not working  )
#endif
//	if (m_exposeRect.intersects(rt))
	if(!m_search)
		p->drawTiledPixmap(rt, px, pt);
}

void Page::drawImage(QPainter * p)
{
	QRectF r=readRect();
	QRectF sr=readRect();
	int flags=readInt();
	QImage img=readImage();
#ifdef __GNUC__
#warning I dont know way but this is not working 
#else
#pragma WARNING( I dont know way but this is not working  )
#endif
//	if (m_exposeRect.intersects(r))
	if(!m_search)
		p->drawImage(r, img, sr, (Qt::ImageConversionFlags)flags);
}


void Page::render(QPaintDevice * device, const QRectF & rect)
{
	QPainter * p=new QPainter(device);
	render(p, rect);
	p->end();
	delete p;
}

void Page::render(QPainter * p, const QRectF & exposeRect)
{
	m_search=false;
	m_exposeRect=exposeRect;
	m_doc->seek(m_pos);
	m_worldTransform=p->worldTransform();
	while(m_doc->pos()<m_toPos)
	{
		char job;
		m_doc->read(&job,1);
		switch(job)
		{
			case 's' :
				drawState(p);
				break;
			case 'i' :
				drawImage(p);
				break;
			case 'd' :
				drawTiledPixmap(p);
				break;
			case 't' :
				drawTextItem(p);
				break;
			case 'x' :
				drawPixmap(p);
				break;

			case 'p' :
				drawPoint(p);
				break;

			case 'h' :
				drawPath(p);
				break;

			case 'e' :
				drawEllipse(p);
				break;

			case 'l' :
				drawLine(p);
				break;

			case 'r' :
				drawRect(p);
				break;
			default:
				qDebug()<<"Houston, we have a problem";
				break;

		}
	}
	m_worldTransform=p->worldTransform();
}

QPrinter::Orientation Page::paperOrientation() const
{
	return m_pageStruct.orientation;
}

QSize Page::pageSize() const
{
	return QSize(m_pageStruct.width, m_pageStruct.height);
}

bool Page::search(const QString &text, QRectF &rect, SearchDirection direction, bool caseSensitive, int rectMargin)
{
	if (!text.length())
		return false;

	if ((caseSensitive && m_searchText != text) || (!caseSensitive && m_searchText.toLower() != text.toLower()))
	{
		if (direction == NextResult)
			m_searchPos=m_pos;
		else
			m_searchPos=m_toPos;
	}

	QList<stringStruct> strings;

	m_searchText = text;

	FakePaintDevice px;//(m_pageStruct.width, m_pageStruct.height,QImage::Format_Mono);
	QPainter * p = new QPainter(&px);
	m_search=true;
	m_exposeRect=QRectF();
	m_doc->seek(m_pos);
	QPointF pt;
	double width, ascent,descent;
	QString st;
	QFont font;
	m_worldTransform=p->worldTransform();

	while(m_doc->pos()<m_toPos)
	{
		char job;
		m_doc->read(&job,1);
		switch(job)
		{
			case 't' :
					pt=readPoint();
					font=readFont();
					readInt();
					width=readDouble();
					ascent=readDouble();
					descent=readDouble();
					st=readString();
					if (st.contains(text, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive))
					{
						stringStruct ss;
						ss.pos=m_doc->pos();
						QRectF textRect=QFontMetricsF(font).boundingRect(st.mid(st.indexOf(text,0, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive), text.length()));
						if (st.indexOf(text,0, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive))
							textRect.translate(QFontMetricsF(font).width(st.left(st.indexOf(text,0, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive))),0);
						textRect.translate(pt);
						textRect=p->clipPath().boundingRect().intersect(textRect);
						ss.textRect=p->transform().mapRect(textRect);
						ss.textRect.adjust(-descent,-descent,descent,descent);
						if (direction == NextResult)
							strings.push_back(ss);
						else
							strings.push_front(ss);
					}
				break;
			case 's' :
				drawState(p);
				break;
			case 'i' :
				drawImage(p);
				break;
			case 'd' :
				drawTiledPixmap(p);
				break;
			case 'x' :
				drawPixmap(p);
				break;

			case 'p' :
				drawPoint(p);
				break;

			case 'h' :
				drawPath(p);
				break;

			case 'e' :
				drawEllipse(p);
				break;

			case 'l' :
				drawLine(p);
				break;

			case 'r' :
				drawRect(p);
				break;
			default:
				qDebug()<<"Houston, we have a problem";
				break;
		}
	}
	delete p;
	foreach (stringStruct st, strings)
		if ((direction == NextResult && st.pos>m_searchPos) || (direction == PreviousResult && st.pos<m_searchPos))
		{
			rect=st.textRect;
			m_searchPos=st.pos;
			return true;
		}
	m_searchText = "";
	return false;
}

QBrush Page::readBrush()
{
	QBrush br;
	br.setColor(readColor());
	br.setTextureImage(readImage());
	br.setStyle((Qt::BrushStyle)readInt());
	int gradient=readInt();
	if (!gradient)
		return br;
	int spread=readInt();
	int coordinateMode=readInt();
	int type=readInt();
	QGradient *gr=0;
	QPointF p1, p2;
	qreal vd;
	switch(type)
	{
		case QGradient::LinearGradient:
			p1=readPoint();
			p2=readPoint();
			gr = new QLinearGradient(p1, p2);
			break;
		case QGradient::RadialGradient:
			p1=readPoint();
			vd=readDouble();
			p2=readPoint();
			gr = new QRadialGradient(p1, vd, p2);
			break;
		case QGradient::ConicalGradient:
			p1=readPoint();
			vd=readDouble();
			gr = new QConicalGradient(p1, vd);
			break;
	}
	gr->setSpread((QGradient::Spread)spread);
	gr->setCoordinateMode((QGradient::CoordinateMode)coordinateMode);
	QGradientStops gsps;
	QGradientStop gsp;
	int stops=readInt();
	for (int i=0;i<stops; i++)
	{
		gsp.first=readDouble();
		gsp.second=readColor();
		gsps.push_back(gsp);
	}
	gr->setStops(gsps);
	return QBrush(*gr);
}

QPointF Page::readPoint()
{
	qreal v1=readDouble();
	qreal v2=readDouble();
	return QPointF(v1, v2);

}
QImage Page::readImage()
{
	QByteArray imd=m_doc->read(readInt());
	QBuffer buffer(&imd);
	buffer.open(QBuffer::ReadWrite);
	QImage img;
	img.load(&buffer, "PNG");
	return img;
}

qreal Page::readDouble()
{
	qreal val;
	m_doc->read((char*)&val, sizeof(qreal));
	return val;
}

int Page::readInt()
{
	int val;
	m_doc->read((char*)&val, sizeof(int));
	return val;
}

QColor Page::readColor()
{
	int r=readInt();
	int g=readInt();
	int b=readInt();
	int a=readInt();
	return QColor(r, g, b, a);
}

QString Page::readString()
{
	return QString::fromUtf8(m_doc->read(readInt()));
}

QFont Page::readFont()
{
	QFont ft;
	ft.fromString(readString());
	return ft;
}

QPen Page::readPen()
{
	QPen pen;
	pen.setBrush(readBrush());
	pen.setColor(readColor());
	pen.setCapStyle((Qt::PenCapStyle)readInt());
	pen.setCosmetic(readInt());
	pen.setJoinStyle((Qt::PenJoinStyle)readInt());
	pen.setMiterLimit(readDouble());
	pen.setWidth(readInt());
	pen.setStyle((Qt::PenStyle)readInt());
	return pen;
}

QRegion Page::readRegion()
{
	QRegion reg;
	int len=readInt();
	while(len)
	{
		reg+=readRect().toRect();
		len--;
	}
	return reg;
}

QRectF Page::readRect()
{
	qreal x=readDouble();
	qreal y=readDouble();
	qreal w=readDouble();
	qreal h=readDouble();
	return QRectF(x, y, w, h);
}

QPainterPath Page::readPath()
{
	int len=readInt();
	int cstate=0;
	QPainterPath p;
	QPointF c1,c2;
	for (int i=0;i<len;i++)
	{
		int type=readInt();
		double x=readDouble();
		double y=readDouble();
		if (type==QPainterPath::MoveToElement)
			p.moveTo(x,y);
		if (type==QPainterPath::LineToElement)
			p.lineTo(x,y);
		if (type==QPainterPath::CurveToElement)
		{
			c1.setX(x);
			c1.setY(y);
			cstate=1;
		}

		if (type==QPainterPath::CurveToDataElement)
		{
			if (cstate==1)
			{
				c2.setX(x);
				c2.setY(y);
				cstate=2;
			}
			else
			{
				p.cubicTo(c1,c2,QPointF(x,y));
				cstate=0;
			}
		}
	}
	return p;
}

}
