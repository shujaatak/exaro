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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QPainter>
#include <QSqlField>
#include <QPainterPath>
#include <QSettings>
 
#include "iteminterface.h"
#include "reportinterface.h"
#include "sqlquery.h"

using namespace Report;

ItemInterface::ItemInterface(QGraphicsItem* parent, QObject * parentObject): QObject(parentObject), QGraphicsItem(parent)
		, m_resizeHandle(2/UNIT) //2mm
		, m_minWidth(m_resizeHandle*2+1), m_minHeight(m_resizeHandle*2+1),m_stretch(0)
{
	m_resizeEvent = Fixed;
	m_resizeFlags = ResizeTop | ResizeBottom | ResizeLeft | ResizeRight;
	m_BGMode = TransparentMode;
	m_width = 20/UNIT; // 20 mm
	m_height = 20/UNIT; // 20 mm
	m_opacity = 100;
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemClipsChildrenToShape);
	QSettings s;
	m_drawSelectionBorder=s.value( "Items/drawSelectionBorder", true ).toBool();
	if (dynamic_cast<ItemInterface*>(parent))
	{
		m_brush=dynamic_cast<ItemInterface*>(parent)->brush();
		m_backgroundBrush=dynamic_cast<ItemInterface*>(parent)->backgroundBrush();
		m_pen=dynamic_cast<ItemInterface*>(parent)->pen();
		m_font=dynamic_cast<ItemInterface*>(parent)->font();
	}
	else
		if(dynamic_cast<PageInterface*>(parentObject))
		{
			m_brush=dynamic_cast<PageInterface*>(parentObject)->foregroundBrush();
			m_backgroundBrush=dynamic_cast<PageInterface*>(parentObject)->backgroundBrush();
			m_font=dynamic_cast<PageInterface*>(parentObject)->font();
		}
}

ItemInterface::~ItemInterface()
{
}

void ItemInterface::drawSelection(QPainter * painter, QRectF rect)
{
	painter->save();
	if (isSelected())
	{
		QBrush a;
		a.setColor(QColor(200,0,0,150));
		a.setStyle(Qt::SolidPattern);
		if (m_drawSelectionBorder)
		{
			QPen p(Qt::DashDotDotLine);
			p.setBrush(a);
			painter->setPen(p);
			painter->drawRect(rect);
		}
		QPainterPath lt;
		lt.moveTo(0,0);
		lt.lineTo(0,m_resizeHandle);
		lt.lineTo(m_resizeHandle,0);
		painter->fillPath(lt,a);

		QPainterPath rt;
		rt.moveTo(rect.width(),0);
		rt.lineTo(rect.width(),m_resizeHandle);
		rt.lineTo(rect.width()-m_resizeHandle,0);
		painter->fillPath(rt,a);

		QPainterPath lb;
		lb.moveTo(0,rect.height());
		lb.lineTo(0,rect.height()-m_resizeHandle);
		lb.lineTo(m_resizeHandle,rect.height());
		painter->fillPath(lb,a);

		QPainterPath rb;
		rb.moveTo(rect.width(),rect.height());
		rb.lineTo(rect.width(),rect.height()-m_resizeHandle);
		rb.lineTo(rect.width()-m_resizeHandle,rect.height());
		painter->fillPath(rb,a);
	}
	else
	{
		if (m_drawSelectionBorder)
		{
			QBrush a;
			a.setColor(QColor(100,100,100,200));
			a.setStyle(Qt::SolidPattern);

			QPen p(Qt::DashDotDotLine);
			p.setBrush(a);
			painter->setPen(p);
			painter->drawRect(rect);
		}
		else
		{
			painter->setPen(QColor(0,0,0,100));
			painter->drawLine(0,0,0,2*m_resizeHandle);
			painter->drawLine(0,0,2*m_resizeHandle,0);
			painter->drawLine(rect.width(),0,rect.width()-2*m_resizeHandle,0);
			painter->drawLine(rect.width(),0,rect.width(),2*m_resizeHandle);
			painter->drawLine(rect.width(),rect.height(),rect.width()-2*m_resizeHandle, rect.height());
			painter->drawLine(rect.width(),rect.height(),rect.width(), rect.height()-2*m_resizeHandle);
			painter->drawLine(0,rect.height(), 2*m_resizeHandle, rect.height());
			painter->drawLine(0,rect.height(), 0, rect.height()-2*m_resizeHandle);
		}
	}

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
	if (!painter)
		return;
	painter->setPen(pen());
	painter->setBackgroundMode((Qt::BGMode)m_BGMode);
	painter->setBackground(backgroundBrush());
	QFont f=font();
	f.setPixelSize(font().pointSizeF()/UNIT);
	painter->setFont(f);
	painter->setOpacity((qreal)m_opacity/100.);
	painter->setBrush(brush());
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
	oldGeometry = geometry();
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
		emit(itemSelected(this, event->pos()));
}

void ItemInterface::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	m_resizeEvent = Fixed;
	
	QRectF newGeometry = geometry();
	if (newGeometry != oldGeometry)
	    emit(geometryChanged(this, newGeometry, oldGeometry));
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

void ItemInterface::prepare(QPainter * painter)
{
	unstretch();
	setupPainter(painter);
	foreach(QObject * obj, QObject::children())
		if (dynamic_cast<Report::ItemInterface*>(obj))
			dynamic_cast<Report::ItemInterface*>(obj)->prepare(painter);
}

void ItemInterface::beginPaint(QPainter * painter)
{
	Q_UNUSED(painter);
	emit beforePrint(this);
}

void ItemInterface::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(widget);
}

void ItemInterface::endPaint(QPainter * painter)
{
	Q_UNUSED(painter);
	emit afterPrint(this);
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
	emit(geometryChanged(geometry()));
}

void ItemInterface::setHeight(qreal height)
{
	prepareGeometryChange();
	m_height = height;
	emit(geometryChanged(geometry()));
}

double ItemInterface::stretch()
{
	return m_stretch;
}

void ItemInterface::setStretch(double val)
{
	if (val>m_stretch)
	{
		m_stretch=val;
		if (dynamic_cast<Report::ItemInterface*>(parentItem()))
			dynamic_cast<Report::ItemInterface*>(parentItem())->setStretch(val);
	}
}

void ItemInterface::unstretch()
{
	m_stretch=0;
	foreach(QObject * obj, QObject::children())
		if (dynamic_cast<Report::ItemInterface*>(obj))
			dynamic_cast<Report::ItemInterface*>(obj)->unstretch();
}


qreal ItemInterface::width() const
{
	return m_width;
}

qreal ItemInterface::height() const
{
	return m_height+m_stretch;
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
		return tr("Field '%1' not found").arg(field);

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
