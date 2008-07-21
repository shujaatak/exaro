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
#include <QVariant>
#include <QBuffer>
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QKeySequence>
#include <QMetaProperty>
#include <QBuffer>

#include "paintdevice.h"
#include "paintengine.h"
#include "globals.h"

namespace Report
{

PaintEngine::PaintEngine(PaintEngineFeatures /*caps*/)
		: QPaintEngine(QPaintEngine::AllFeatures), m_document(0)
{
}


bool PaintEngine::begin(QPaintDevice *pdev)
{
	m_paintDevice = dynamic_cast<PaintDevice *>(pdev);
	return m_paintDevice;
}

void PaintEngine::setPageNode(QDomNode & pageNode)
{
	m_pageNode = pageNode;
}

void PaintEngine::setDomDocument(QDomDocument* document)
{
	m_document = document;
}

bool PaintEngine::end()
{
	if (!m_paintDevice)
		return false;
	m_paintDevice->endDoc();
	return true;
}

void PaintEngine::updateState(const QPaintEngineState &state)
{
	QDomElement el = m_document->createElement("stateChanged");

	if (state.state() & QPaintEngine::DirtyHints)
		el.appendChild(variatToDom(m_document, "renderHints", (int)state.renderHints()));

	if (state.state() & QPaintEngine::DirtyTransform)
		el.appendChild(variatToDom(m_document, "transform", state.transform()));

	if (state.state() & QPaintEngine::DirtyBackground)
		el.appendChild(variatToDom(m_document, "backgroundBrush", state.backgroundBrush()));

	if (state.state() & QPaintEngine::DirtyBackgroundMode)
		el.appendChild(variatToDom(m_document, "backgroundMode", state.backgroundMode()));

	if (state.state() & QPaintEngine::DirtyBrush)
		el.appendChild(variatToDom(m_document, "brush", state.brush()));

	if (state.state() & QPaintEngine::DirtyBrushOrigin)
		el.appendChild(variatToDom(m_document, "brushOrigin", state.brushOrigin()));

	if (state.state() & QPaintEngine::DirtyCompositionMode)
		el.appendChild(variatToDom(m_document, "compositionMode", state.compositionMode()));

	if (state.state() & QPaintEngine::DirtyFont)
		el.appendChild(variatToDom(m_document, "font", state.font()));

	if (state.state() & QPaintEngine::DirtyOpacity)
		el.appendChild(variatToDom(m_document, "opacity", state.opacity()));

	if (state.state() & QPaintEngine::DirtyPen)
		el.appendChild(variatToDom(m_document, "pen", state.pen()));

	if (state.state() & QPaintEngine::DirtyClipRegion)
	{
		el.appendChild(variatToDom(m_document, "clipOperation", state.clipOperation()));
		el.appendChild(variatToDom(m_document, "clipRegion", state.clipRegion()));
	}

	if (state.state() & QPaintEngine::DirtyClipPath)
	{
		el.appendChild(variatToDom(m_document, "clipOperation", state.clipOperation()));
		QVariant v;
		v.setValue(state.clipPath());
		el.appendChild(variatToDom(m_document, "clipPath", v));
	}

	if (state.state() & QPaintEngine::DirtyClipEnabled)
		el.appendChild(variatToDom(m_document, "isClipEnabled", state.isClipEnabled()));

	m_pageNode.appendChild(el);
}

void PaintEngine::drawRects(const QRect *rects, int rectCount)
{
	for (int i = 0;i < rectCount;i++)
		m_pageNode.appendChild(variatToDom(m_document, "rect", QRectF(rects[i].x() , rects[i].y(), rects[i].width(), rects[i].height())));
}

void PaintEngine::drawRects(const QRectF *rects, int rectCount)
{
	for (int i = 0;i < rectCount;i++)
		m_pageNode.appendChild(variatToDom(m_document, "rect", QRectF(rects[i].x(), rects[i].y(), rects[i].width(), rects[i].height())));
}

void PaintEngine::drawLines(const QLine *lines, int lineCount)
{
	for (int i = 0;i < lineCount;i++)
		m_pageNode.appendChild(variatToDom(m_document, "line", QLineF(lines[i].x1(), lines[i].y1(), lines[i].x2(), lines[i].y2())));
}

void PaintEngine::drawLines(const QLineF *lines, int lineCount)
{
	for (int i = 0;i < lineCount;i++)
		m_pageNode.appendChild(variatToDom(m_document, "line", QLineF(lines[i].x1(), lines[i].y1(), lines[i].x2(), lines[i].y2())));
}

void PaintEngine::drawEllipse(const QRectF &r)
{
	m_pageNode.appendChild(variatToDom(m_document, "ellipse", r));
}
void PaintEngine::drawEllipse(const QRect &r)
{
	m_pageNode.appendChild(variatToDom(m_document, "ellipse", QRectF(r)));
}

void PaintEngine::drawPath(const QPainterPath &path)
{
	QVariant var;
	var.setValue(path);
	m_pageNode.appendChild(variatToDom(m_document, "path",var));
}

void PaintEngine::drawPoints(const QPointF *points, int pointCount)
{
	for (int i = 0;i < pointCount;i++)
		m_pageNode.appendChild(variatToDom(m_document, "point", points[i]));
}

void PaintEngine::drawPoints(const QPoint *points, int pointCount)
{
	for (int i = 0;i < pointCount;i++)
		m_pageNode.appendChild(variatToDom(m_document, "point", points[i]));
}

void PaintEngine::drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
	if (QPaintEngine::PolylineMode != mode)
	{
		qWarning() << "drawPolygon is not fully implemented. Only PolylineMode is working";
		return;
	}

	for (int i = 1;i < pointCount;i++)
	{
		QLineF line = QLineF(points[i-1].x(), points[i-1].y(), points[i].x(), points[i].y());
		drawLines(&line, 1);
	}
}

void PaintEngine::drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode)
{
	if (QPaintEngine::PolylineMode != mode)
	{
		qWarning() << "drawPolygon is not fully implemented. Only PolylineMode is working";
		return;
	}

	for (int i = 1;i < pointCount;i++)
	{
		QLine line = QLine(points[i-1].x(), points[i-1].y(), points[i].x(), points[i].y());
		drawLines(&line, 1);
	}
}

void PaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
	QDomElement el = m_document->createElement("pixmap");
	el.appendChild(variatToDom(m_document, "toRect", r));
	el.appendChild(variatToDom(m_document, "sourceRect", sr));
	el.appendChild(variatToDom(m_document, "img", pm));
	m_pageNode.appendChild(el);
}

void PaintEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
	QDomElement el = m_document->createElement("text");
	el.appendChild(variatToDom(m_document, "toPoint", p));
	el.appendChild(variatToDom(m_document, "font", textItem.font()));
	el.appendChild(variatToDom(m_document, "renderFlags", (int)textItem.renderFlags()));
	el.appendChild(variatToDom(m_document, "width", textItem.width()));
	el.appendChild(variatToDom(m_document, "ascent", textItem.ascent()));
	el.appendChild(variatToDom(m_document, "descent", textItem.descent()));
	el.appendChild(variatToDom(m_document, "string", textItem.text()));
	m_pageNode.appendChild(el);
}

void PaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pm, const QPointF &s)
{
	QDomElement el = m_document->createElement("tiledPixmap");
	el.appendChild(variatToDom(m_document, "toRect", r));
	el.appendChild(variatToDom(m_document, "toPoint", s));
	el.appendChild(variatToDom(m_document, "img", pm));
	m_pageNode.appendChild(el);
}

void PaintEngine::drawImage(const QRectF &r, const QImage &pm, const QRectF &sr,
                            Qt::ImageConversionFlags flags)
{
	QDomElement el = m_document->createElement("image");
	el.appendChild(variatToDom(m_document, "toRect", r));
	el.appendChild(variatToDom(m_document, "sourceRect", sr));
	el.appendChild(variatToDom(m_document, "flags", (int)flags));
	el.appendChild(variatToDom(m_document, "img", pm));
	m_pageNode.appendChild(el);
}

QPaintEngine::Type PaintEngine::type() const
{
	return QPaintEngine::User;
}

PaintEngine::~PaintEngine()
{
}

}
