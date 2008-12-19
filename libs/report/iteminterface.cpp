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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QPainter>
#include <QSqlField>

#include "iteminterface.h"
#include "reportinterface.h"
#include "sqlquery.h"

using namespace Report;

ItemInterface::ItemInterface(QGraphicsItem* parent, QObject * parentObject): QObject(parentObject), QGraphicsItem(parent)
		, m_resizeHandle(2/UNIT) //2mm
		, m_minWidth(m_resizeHandle*2+1), m_minHeight(m_resizeHandle*2+1)
{
	m_resizeEvent = Fixed;
	m_resizeFlags = ResizeTop | ResizeBottom | ResizeLeft | ResizeRight;
	m_BGMode = TransparentMode;
	m_width = 20/UNIT; // 20 mm
	m_height = 20/UNIT; // 20 mm
	m_opacity = 100;
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemClipsChildrenToShape);
	m_font=QFont("Serif");
	m_font.setPointSizeF(3.5);
	m_font.setStyleStrategy(QFont::PreferMatch);
	m_font.setStyleStrategy(QFont::ForceOutline);
}

ItemInterface::~ItemInterface()
{
}


void ItemInterface::drawSelection(QPainter * painter, QRectF rect)
{
	painter->save();
	QBrush a;

	if (isSelected())
	{
		a.setColor(QColor(100, 100, 100, 100));
		a.setStyle(Qt::SolidPattern);
	}

	painter->setBrush(a);

	painter->setPen(Qt::DashDotDotLine);
	painter->drawRect(rect);
	painter->restore();
}

QBrush ItemInterface::brush()
{
	return m_brush;
}

void ItemInterface::setBrush(const QBrush & brush)
{
	m_brush = brush;
	update();
}

QBrush ItemInterface::backgroundBrush()
{
	return m_backgroundBrush;
}

void ItemInterface::setBackgroundBrush(const QBrush & brush)
{
	m_backgroundBrush = brush;
	update();
}


QPen ItemInterface::pen()
{
	return m_pen;
}

void ItemInterface::setPen(const QPen & pen)
{
	m_pen = pen;
	update();
}

QFont ItemInterface::font()
{
	return m_font;
}

void ItemInterface::setFont(const QFont & font)
{
	m_font = font;
	update();
}


void ItemInterface::setupPainter(QPainter * painter)
{
	painter->setBrush(brush());
	painter->setPen(pen());
	painter->setBackgroundMode((Qt::BGMode)m_BGMode);
	painter->setBackground(backgroundBrush());
	QFont f=font();(font().family());
	f.setPixelSize(font().pointSizeF()/UNIT);
	painter->setFont(f);
	painter->setOpacity((qreal)m_opacity/100.);
}

int ItemInterface::resizeFlags()
{
	return m_resizeFlags;
}

void ItemInterface::setResizeFlags(int resizeFlags)
{
	m_resizeFlags = resizeFlags;
}

void ItemInterface::setResizeHandle(int resizeHandle)
{
	m_resizeHandle = resizeHandle;

	if (m_minWidth<m_resizeHandle*2+1)
		m_minWidth=m_resizeHandle*2+1;

	if (m_minHeight<m_resizeHandle*2+1)
		m_minHeight=m_resizeHandle*2+1;

	update(boundingRect());
}

int ItemInterface::resizeHandle()
{
	return m_resizeHandle;
}

void ItemInterface::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	foreach(QGraphicsItem *item, scene()->items())

	if (item->zValue() == 1)
		item->setZValue(0);

	setZValue(1);

	if (event->buttons() == Qt::LeftButton)
		m_resizeEvent = posibleResizeCurrsor(event->pos());
	else
		m_resizeEvent = Fixed;

	if (m_resizeEvent == Fixed)
		setCursor(QCursor(Qt::ClosedHandCursor));

	QGraphicsItem::mousePressEvent(event);

	if (event->buttons() == Qt::LeftButton)
		emit itemSelected(this, event->pos());
}

void ItemInterface::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	m_resizeEvent = Fixed;
}

int ItemInterface::posibleResizeCurrsor(QPointF cursor)
{
	int flags = Fixed;
	QRectF topRect(0, 0, width(), resizeHandle());
	QRectF leftRect(0, 0, resizeHandle(), height());
	QRectF bottomRect(0, height() - resizeHandle(), width(), resizeHandle());
	QRectF rightRect(width() - resizeHandle(), 0, width(), height());

	if (resizeFlags()&ResizeTop && topRect.contains(cursor))
		flags |= ResizeTop;

	if (resizeFlags()&ResizeLeft && leftRect.contains(cursor))
		flags |= ResizeLeft;

	if (resizeFlags()&ResizeBottom && bottomRect.contains(cursor))
		flags |= ResizeBottom;

	if (resizeFlags()&ResizeRight && rightRect.contains(cursor))
		flags |= ResizeRight;

	if (resizeFlags()&FixedPos)
		flags |= FixedPos;

	return flags;
}

void ItemInterface::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(widget);
}

bool ItemInterface::canContain(QObject * object)
{
	Q_UNUSED(object);
	return false;
}

void ItemInterface::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		if (m_resizeEvent == Fixed)
			setPos(pos() + QPoint((int)(event->scenePos().x() - event->lastScenePos().x()), (int)(event->scenePos().y() - event->lastScenePos().y())));
		else
		{
			if (m_resizeEvent&ResizeLeft)
			{
				setPos(pos().x() + event->scenePos().x() - event->lastScenePos().x(), pos().y());
				setWidth(width() + event->lastScenePos().x() - event->scenePos().x());
			}

			if (m_resizeEvent&ResizeTop)
			{
				setPos(pos().x(), pos().y() + event->scenePos().y() - event->lastScenePos().y());
				setHeight(height() + event->lastScenePos().y() - event->scenePos().y());
			}

			if (m_resizeEvent&ResizeRight)
				setWidth((int)(width() + event->scenePos().x() - event->lastScenePos().x()));

			if (m_resizeEvent&ResizeBottom)
				setHeight((int)(height() + event->scenePos().y() - event->lastScenePos().y()));

			if (width() < minWidth())
				setWidth(minWidth());

			if (height() < minHeight())
				setHeight(minHeight());
		}
	}

	QGraphicsItem::mouseMoveEvent(event);
}

ItemInterface::BGMode ItemInterface::backgroundMode()
{
	return m_BGMode;
}

void ItemInterface::setBackgroundMode(BGMode bgMode)
{
	m_BGMode = bgMode;
	update(boundingRect());
}

void ItemInterface::setWidth(qreal width)
{
	prepareGeometryChange();
	m_width = width;
	emit geometryChanged(geometry());
}

void ItemInterface::setHeight(qreal height)
{
	prepareGeometryChange();
	m_height = height;
	emit geometryChanged(geometry());
}

qreal ItemInterface::width() const
{
	return m_width;
}

qreal ItemInterface::height() const
{
	return m_height;
}

QRectF ItemInterface::geometry()
{
	return QRectF(pos().x(), pos().y(), width(), height());
}

void ItemInterface::setGeometry(QRectF rect)
{
	setPos(rect.x(), rect.y());
	setWidth(rect.width());
	setHeight(rect.height());
}

void ItemInterface::setMinHeight(qreal height)
{
    height > 0 ? m_minHeight = height: 0;

	if (m_resizeHandle*2+1<m_minHeight)
		setResizeHandle(m_minHeight/2-1);
}

void ItemInterface::setMinWidth(qreal width)
{
    width > 0 ? m_minWidth = width: 0;

	if (m_resizeHandle*2+1<m_minWidth)
		setResizeHandle(m_minWidth/2-1);
}

qreal ItemInterface::minHeight() const
{
    return m_minHeight;
}

qreal ItemInterface::minWidth() const
{
    return m_minWidth;
}

QRectF ItemInterface::adjustRect(QRectF & rect)
{
	qreal penwidth = pen().widthF();
	rect=rect.adjusted(penwidth,penwidth,-penwidth,-penwidth);
	return rect;
}

SqlQuery * ItemInterface::findQuery(const QString & query)
{
	QObject * p = parent();
	while (p)
	{
		if (dynamic_cast<ReportInterface*>(p))
			break;
		p = p->parent();
	}

	if (p)
		return p->findChild<SqlQuery *>(query);
	return 0;
}

QVariant ItemInterface::queryField(const QString & query, const QString & field)
{
	SqlQuery * qry = findQuery(query);

	if (!qry)
		return tr("Query '%1' not found").arg(query);

	if (!qry->record().field(field).isValid())
		return tr("Field '%1' not found").arg(query);

	return qry->value(field);
}

int ItemInterface::opacity()
{
	return m_opacity;
}

void ItemInterface::setOpacity(const int opacity)
{
	if (opacity < 0)
		m_opacity = 0;
	else
		if (opacity > 100)
			m_opacity = 100;
		else
			m_opacity =  opacity;

	update();
}

QRectF ItemInterface::parentGeometry()
{
	if (dynamic_cast<Report::ItemInterface*>(parentItem()))
		return QRectF(0, 0, dynamic_cast<Report::ItemInterface*>(parentItem())->geometry().width(), dynamic_cast<Report::ItemInterface*>(parentItem())->geometry().height() - 1);
	else
		if (dynamic_cast<Report::PageInterface*>(scene()))
			return dynamic_cast<Report::PageInterface*>(scene())->geometry();
	return geometry();
}

QScriptEngine * ItemInterface::scriptEngine()
{
	ReportInterface	* p = reportObject();

	if (p)
		return p->scriptEngine();
	else
		return 0;
}

ReportInterface	* ItemInterface::reportObject()
{
	QObject * p = parent();
	while (p)
	{
		if (dynamic_cast<ReportInterface*>(p))
			break;
		p = p->parent();
	}

	if (p)
		return (dynamic_cast<ReportInterface*>(p));
	return 0;
}
