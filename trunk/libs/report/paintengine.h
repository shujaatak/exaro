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
#ifndef REPORTPAINTENGINE_H
#define REPORTPAINTENGINE_H

#include <QPaintEngine>
#include <QDomNode>

namespace Report
{

class PaintDevice;;

class PaintEngine : public QPaintEngine
{
private:
	void setDomDocument(QDomDocument* document);
	void setPageNode(QDomNode & pageNode);
	PaintEngine(PaintEngineFeatures caps = 0);
	friend class PaintDevice;

public:
	~PaintEngine();

	bool begin(QPaintDevice *pdev);
	bool end();

	void updateState(const QPaintEngineState &state);

	void drawRects(const QRect *rects, int rectCount);
	void drawRects(const QRectF *rects, int rectCount);

	void drawLines(const QLine *lines, int lineCount);
	void drawLines(const QLineF *lines, int lineCount);

	void drawEllipse(const QRectF &r);
	void drawEllipse(const QRect &r);

	void drawPath(const QPainterPath &path);

	void drawPoints(const QPointF *points, int pointCount);
	void drawPoints(const QPoint *points, int pointCount);

	void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
	void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode);

	void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);

	void drawTextItem(const QPointF &p, const QTextItem &textItem);

	void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s);

	void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr, Qt::ImageConversionFlags flags = Qt::AutoColor);

	QPaintEngine::Type type() const;

private:
	QDomDocument * m_document;
	QDomNode m_pageNode;
	PaintDevice * m_paintDevice;
};

}

#endif
