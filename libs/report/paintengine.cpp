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

#include <QVariant>
#include <QBuffer>
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
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

PaintEngine::PaintEngine(QIODevice * doc)
		: QPaintEngine(QPaintEngine::AllFeatures), m_document(doc)
{
}
bool PaintEngine::begin(QPaintDevice *pdev)
{
	return true;
}

bool PaintEngine::end()
{
	return true;
}

void PaintEngine::updateState(const QPaintEngineState &state)
{
	writeChar('s');
	writeInt(state.state());

	if (state.state() & QPaintEngine::DirtyHints)
		writeInt(state.renderHints());

	if (state.state() & QPaintEngine::DirtyTransform)
	{
		writeDouble(state.transform().m11());
		writeDouble(state.transform().m12());
		writeDouble(state.transform().m13());
		writeDouble(state.transform().m21());
		writeDouble(state.transform().m22());
		writeDouble(state.transform().m23());
		writeDouble(state.transform().m31());
		writeDouble(state.transform().m32());
		writeDouble(state.transform().m33());
	}

	if (state.state() & QPaintEngine::DirtyBackground)
		writeBrush(state.backgroundBrush());

	if (state.state() & QPaintEngine::DirtyBackgroundMode)
		writeInt(state.backgroundMode());

	if (state.state() & QPaintEngine::DirtyBrush)
		writeBrush(state.brush());

	if (state.state() & QPaintEngine::DirtyBrushOrigin)
		writePoint(state.brushOrigin());

	if (state.state() & QPaintEngine::DirtyCompositionMode)
		writeInt(state.compositionMode());

	if (state.state() & QPaintEngine::DirtyFont)
		writeFont(state.font());

	if (state.state() & QPaintEngine::DirtyOpacity)
		writeDouble(state.opacity());

	if (state.state() & QPaintEngine::DirtyPen)
		writePen(state.pen());

	if (state.state() & QPaintEngine::DirtyClipRegion)
	{
		writeRegion(state.clipRegion());
		writeInt(state.clipOperation());
	}

	if (state.state() & QPaintEngine::DirtyClipPath)
	{
		writePath(state.clipPath());
		writeInt(state.clipOperation());
	}

	if (state.state() & QPaintEngine::DirtyClipEnabled)
		writeInt(state.isClipEnabled());
}

void PaintEngine::drawRects(const QRect *rects, int rectCount)
{
	writeChar('r');
	writeInt(rectCount);
	for (int i = 0;i < rectCount;i++)
		writeRect(rects[i]);
}

void PaintEngine::drawRects(const QRectF *rects, int rectCount)
{
	writeChar('r');
	writeInt(rectCount);
	for (int i = 0;i < rectCount;i++)
		writeRect(rects[i]);
}

void PaintEngine::drawLines(const QLine *lines, int lineCount)
{
	writeChar('l');
	writeInt(lineCount);
	for (int i = 0;i < lineCount;i++)
		writeLine(lines[i]);
}

void PaintEngine::drawLines(const QLineF *lines, int lineCount)
{
	writeChar('l');
	writeInt(lineCount);
	for (int i = 0;i < lineCount;i++)
		writeLine(lines[i]);
}

void PaintEngine::drawEllipse(const QRectF &r)
{
	writeChar('e');
 	writeRect(r);
}
void PaintEngine::drawEllipse(const QRect &r)
{
	writeChar('e');
	writeRect(r);
}

void PaintEngine::drawPath(const QPainterPath &path)
{
	writeChar('h');
	writePath(path);
}

void PaintEngine::drawPoints(const QPointF *points, int pointCount)
{
	writeChar('p');
	writeInt(pointCount);
	for (int i = 0;i < pointCount;i++)
		writePoint(points[i]);
}

void PaintEngine::drawPoints(const QPoint *points, int pointCount)
{
	writeChar('p');
	writeInt(pointCount);
	for (int i = 0;i < pointCount;i++)
		writePoint(points[i]);
}

void PaintEngine::drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
	QPainterPath p;

	switch(mode) 
	{
		case OddEvenMode:
			p.setFillRule(Qt::OddEvenFill);
			break;
		case ConvexMode:
		case WindingMode:
			p.setFillRule(Qt::WindingFill);
			break;
		default:
			break;
	}

	p.moveTo(points[0]);
	for (int i = 1; i < pointCount; ++i)
		p.lineTo(points[i]);

	if (mode != PolylineMode)
		p.closeSubpath();
	drawPath(p);
}

void PaintEngine::drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode)
{
	QPainterPath p;

	switch(mode) 
	{
		case OddEvenMode:
			p.setFillRule(Qt::OddEvenFill);
			break;
		case ConvexMode:
		case WindingMode:
			p.setFillRule(Qt::WindingFill);
			break;
		default:
			break;
	}

	p.moveTo(points[0]);
	for (int i = 1; i < pointCount; ++i)
		p.lineTo(points[i]);

	if (mode != PolylineMode)
		p.closeSubpath();
	drawPath(p);
}

void PaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
	writeChar('x');
	writeRect(r);
	writeRect(sr);
	writeImage(pm.toImage());
}

void PaintEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
	writeChar('t');
	writePoint(p);
	writeFont(textItem.font());
	writeInt(textItem.renderFlags());
	writeDouble(textItem.width());
	writeDouble(textItem.ascent());
	writeDouble(textItem.descent());
	writeString(textItem.text());
}

void PaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pm, const QPointF &s)
{
	writeChar('d');
	writeRect(r);
	writePoint(s);
	writeImage(pm.toImage());
}

void PaintEngine::drawImage(const QRectF &r, const QImage &pm, const QRectF &sr,
                            Qt::ImageConversionFlags flags)
{
	writeChar('i');
	writeRect(r);
	writeRect(sr);
	writeInt(flags);
	writeImage(pm);
}

QPaintEngine::Type PaintEngine::type() const
{
	return QPaintEngine::User;
}

PaintEngine::~PaintEngine()
{
}

void PaintEngine::writeImage(const QImage& img)
{
	QByteArray data;
	QBuffer buffer(&data);
	buffer.open(QBuffer::ReadWrite);
	img.save(&buffer, "PNG");
	writeInt(data.size());
	m_document->write(data);
	buffer.close();
}

void PaintEngine::writeColor(const QColor& color)
{
	writeInt(color.red());
	writeInt(color.green());
	writeInt(color.blue());
	writeInt(color.alpha());
}

void PaintEngine::writeString(const QString & string)
{
	writeInt(string.toUtf8().size());
	m_document->write(string.toUtf8());
}

void PaintEngine::writeFont(const QFont& font)
{
	writeString(font.toString());
}

void PaintEngine::writeBrush(const QBrush& brush)
{
	writeColor(brush.color());
	writeImage(brush.textureImage());
	writeInt(brush.style());
	writeInt(brush.gradient() && brush.gradient()->type()!=QGradient::NoGradient);
	if (brush.gradient() && brush.gradient()->type()!=QGradient::NoGradient)
	{
		writeInt(brush.gradient()->spread());
		writeInt(brush.gradient()->coordinateMode());
		writeInt(brush.gradient()->type());
		switch(brush.gradient()->type())
		{
			case QGradient::LinearGradient:
				writePoint(reinterpret_cast<const QLinearGradient*>(brush.gradient())->start());
				writePoint(reinterpret_cast<const QLinearGradient*>(brush.gradient())->finalStop());
				break;
			case QGradient::RadialGradient:
				writePoint(reinterpret_cast<const QRadialGradient*>(brush.gradient())->center());
				writeDouble(reinterpret_cast<const QRadialGradient*>(brush.gradient())->radius());
				writePoint(reinterpret_cast<const QRadialGradient*>(brush.gradient())->focalPoint());
				break;
			case QGradient::ConicalGradient:
				writePoint(reinterpret_cast<const QConicalGradient*>(brush.gradient())->center());
				writeDouble(reinterpret_cast<const QConicalGradient*>(brush.gradient())->angle());
				break;
			default:
				break;
		}
		writeInt(brush.gradient()->stops().size());
		foreach(QGradientStop stop, brush.gradient()->stops())
		{
			writeDouble(stop.first);
			writeColor(stop.second);
		}
	}
}

void PaintEngine::writePen(const QPen& pen)
{
	writeBrush(pen.brush());
	writeColor(pen.color());
	writeInt(pen.capStyle());
	writeInt(pen.isCosmetic());
	writeInt(pen.joinStyle());
	writeDouble(pen.miterLimit());
	writeInt(pen.width());
	writeInt(pen.style());
}

void PaintEngine::writeRect(const QRectF& rect)
{
	writeDouble(rect.x());
	writeDouble(rect.y());
	writeDouble(rect.width());
	writeDouble(rect.height());
}

void PaintEngine::writePoint(const QPointF& point)
{
	writeDouble(point.x());
	writeDouble(point.y());
}

void PaintEngine::writeLine(const QLineF& line)
{
	writeDouble(line.x1());
	writeDouble(line.y1());
	writeDouble(line.x2());
	writeDouble(line.y2());
}

void PaintEngine::writeRegion(const QRegion& reg)
{
	writeInt(reg.rects().size());
	foreach(QRect r, reg.rects())
		writeRect(r);
}

void PaintEngine::writeInt(const int & val )
{
	m_document->write((char*)&val, sizeof(int));
}

void PaintEngine::writeDouble(const qreal & val )
{
	m_document->write((char*)&val, sizeof(qreal));
}

void PaintEngine::writeChar(const char & val )
{
	m_document->write((char*)&val, sizeof(char));
}

void PaintEngine::writePath(const QPainterPath& path)
{
	writeInt(path.elementCount());
	for (int i=0;i<path.elementCount();i++)
	{
		writeInt(path.elementAt(i).type);
		writeDouble(path.elementAt(i).x);
		writeDouble(path.elementAt(i).y);
	}
}

}
