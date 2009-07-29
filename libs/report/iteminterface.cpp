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

using namespace Report;

ItemInterface::ItemInterface(QGraphicsItem* parent, QObject * parentObject): QObject(parentObject), QGraphicsItem(parent)
		, m_resizeHandle(2/UNIT) //2mm
		, m_minWidth(m_resizeHandle*2+1), m_minHeight(m_resizeHandle*2+1),m_stretch(0)
		, m_paintInterface(0)
		, offsetX(0), offsetY(0), _offsetX(0), _offsetY(0)
{
    	m_frame = 0;
	m_resizeEvent = Fixed;
	m_resizeFlags = ResizeTop | ResizeBottom | ResizeLeft | ResizeRight;
	m_width = 20/UNIT; // 20 mm
	m_height = 20/UNIT; // 20 mm
	m_opacity = 100;
	m_enabled = true;
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemClipsChildrenToShape);
	QSettings s;
	m_drawSelectionBorder=s.value( "Items/drawSelectionBorder", true ).toBool();
	expBegin = "[";
	expEnd = "]";
	m_BGMode = TransparentMode;
}


ItemInterface::~ItemInterface()
{
}

ItemInterface::Frames ItemInterface::frame()
{
	return m_frame;
}

void ItemInterface::setFrame(Frames frame)
{
	m_frame = frame;
	update();
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

	painter->restore();
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
		emit(itemSelected(this, event->pos(), event->modifiers()  ) );
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

void ItemInterface::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    /// common item paint block

    if (option->type != QStyleOption::SO_GraphicsItem)
	emit beforePrint(this);

    if (!painter)
	return;

    painter->save();
    painter->setOpacity((qreal)m_opacity/100.);
    painter->setRenderHint(QPainter::TextAntialiasing);

    QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;
    qreal penwidth = m_borderPen.widthF();
    rect=rect.adjusted(penwidth,penwidth,-penwidth,-penwidth);

    painter->save();
    painter->setBrush(backgroundBrush());
    painter->setPen( QPen(Qt::NoPen));
    painter->drawRect(rect);
    painter->restore();

    /// own item painting

    _paint(painter, option, rect, widget);

    /// common item paint block

    painter->setPen(m_borderPen);

    if (frame()&DrawLeft)
	painter->drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());

    if (frame()&DrawRight)
	painter->drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());

    if (frame()&DrawTop)
	painter->drawLine(rect.left(), rect.top(), rect.right(), rect.top());

    if (frame()&DrawBottom)
	painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());

    if (option->type == QStyleOption::SO_GraphicsItem)
	drawSelection(painter, boundingRect());

    painter->restore();

    if (option->type != QStyleOption::SO_GraphicsItem)
	emit afterPrint(this);    
}
void ItemInterface::_paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QRectF & rect, QWidget * widget)
{
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(rect);
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

DataSet * ItemInterface::findDataset(const QString & dataset)
{
	QObject * p = parent();
	while (p)
	{
		if (dynamic_cast<ReportInterface*>(p))
			break;
		p = p->parent();
	}

	if (p)
		return p->findChild<DataSet *>(dataset);
	return 0;
}

QVariant ItemInterface::datasetField(const QString & dataset, const QString & field)
{
	DataSet * dtst = findDataset(dataset);

	if (!dtst)
		return tr("Query '%1' not found").arg(dataset);

	if (!dtst->value(field).isValid())
		return tr("Field '%1' not found").arg(field);

	return dtst->value(field);
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

bool ItemInterface::isEnabled()
{
    return m_enabled;
}

void ItemInterface::setEnabled(bool e)
{
    m_enabled = e;
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

ItemInterface::BGMode ItemInterface::backgroundMode()
{
	return m_BGMode;
}

void ItemInterface::setBackgroundMode(BGMode bgMode)
{
	m_BGMode = bgMode;
	update(boundingRect());
}

QPen ItemInterface::borderPen()
{
	return m_borderPen;
}

void ItemInterface::setBorderPen(const QPen & pen)
{
	m_borderPen = pen;
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


QFont ItemInterface::fontConvert(QFont & font)
{
    QFont f(font);
    f.setPixelSize(f.pointSizeF()/UNIT);
    return f;
}

const QRectF ItemInterface::adjustRect(QRectF & rect, const QPen & pen)
{
	qreal penwidth = pen.widthF();
	rect=rect.adjusted(penwidth,penwidth,-penwidth,-penwidth);
	return rect;
}

void ItemInterface::raise()
{
    	foreach(QGraphicsItem *item, scene()->items())
		if (item->zValue() == 1)
			item->setZValue(0);

	setZValue(1);
}

bool ItemInterface::prInit(PaintInterface * paintInterface)
{
    m_paintInterface = paintInterface;
    return true;
}

bool ItemInterface::prData(){return true;}

bool ItemInterface::prPaint(QPainter * painter, QPointF translate, const QRectF & clipRect)
{
//    qDebug("prPaint. item=%s",qPrintable (this->objectName()));
    emit beforePrint(this);

    QStyleOptionGraphicsItem option;
    option.type = 31;
    painter->save();
    painter->resetMatrix();
    painter->translate(translate);
    option.exposedRect = dynamic_cast<BandInterface *>(this) ? QRectF(0, 0, geometry().width(), geometry().height()) : geometry();
    translate += option.exposedRect.topLeft() + QPointF(offsetX + _offsetX, offsetY + _offsetY);
//    painter->setClipRect(clipRect);
    paint(painter, &option);
    painter->restore();

    foreach(QObject * child, QObject::children())
    {
    if (ItemInterface * childItem = dynamic_cast<ItemInterface *>(child))
	if (childItem->prData())
	    childItem->prPaint(painter, translate, option.exposedRect);
    }
    emit afterPrint(this);

    /*
    QStyleOptionGraphicsItem option;
    option.type = 31;
    option.exposedRect = dynamic_cast<BandInterface *>(this) ? QRectF(0, 0, dynamic_cast<BandInterface *>(this)->geometry().width(), dynamic_cast<BandInterface *>(this)->geometry().height()) : geometry();
    painter->save();
    option.exposedRect.translate(translate);
    painter->setClipRect(clipRect);

    paint(painter, &option);

    painter->restore();
    translate += option.exposedRect.topLeft() + QPointF(offsetX + _offsetX, offsetY + _offsetY);
    foreach(ItemInterface * childItem, findChildren<ItemInterface *>())
	if (childItem->prData())
	    childItem->prPaint(painter, translate, option.exposedRect);
	    
*/
}

bool ItemInterface::prReset(){return false;}


QString ItemInterface::lastError()
{
    return m_lastError;
}

QString ItemInterface::setExpressionDelimeters(QString str)
{
    expBegin = str.section(",",0,0);
    expEnd = str.section(",",1,1);
}

QString ItemInterface::expressionDelimeters()
{
    return QString("%1,%2").arg(expBegin).arg(expEnd);
}

bool ItemInterface::stringIsField(QString str)
{
    QRegExp reField("\\w+\\b*\\.{1}\\\".*\\\"");
    int pos = reField.indexIn(str);
    if (pos > -1)
	return true;
    else
	return false;
}

bool ItemInterface::stringToField (QString str, QString * query, QString * field)
{
    bool ok = false;
    QRegExp reField("\\w+\\b*\\.{1}\\\".*\\\"");
    int pos = reField.indexIn(str);
    if (pos > -1)
    {
	*query = reField.cap(0).section(".",0,0);
	*field = reField.cap(0).section(".",1,1).remove("\"");
	ok = true;
    }
    return ok;
}


QString ItemInterface::processString(QString str)
{
    while (str.contains(expBegin))
    {
	QString firstPart = str.section(expBegin,0,0);
	QString secondPart = str.section(expBegin,1);
	QString insertion = secondPart.section(expEnd,0,0);

	QString fourPart = secondPart.section(expEnd,1);

	insertion = calculateAgregateFunctions(insertion, this);

	QRegExp reField("\\w+\\b*\\.{1}\\\".*\\\"");
	reField.setMinimal(true);

	int pos = 0;
	while ((pos = reField.indexIn(insertion, pos)) != -1)
	{
	    QString dataset = reField.cap(0).section(".",0,0);
	    QString field = reField.cap(0).section(".",1,1).remove("\"");
	    insertion.replace(reField.cap(0), datasetField(dataset,field).toString());
	    pos += reField.matchedLength();
	}
	
	QString evaluateStr;
	#warning 'FIXMI: need optimize checking for script present'
	if (scriptEngine()->canEvaluate(insertion))
		evaluateStr = scriptEngine()->evaluate(insertion).toString();
	if (!scriptEngine()->hasUncaughtException())
	    insertion = evaluateStr;
//	else qWarning("QtScript has exeption in:\n%s",qPrintable(evaluateStr));

	str = firstPart + insertion + fourPart;
    }
    return str;
}

QStringList ItemInterface::getAgregateFunctionsFromString(QString str)
{
    QStringList list;
    QRegExp reField("(sum|min|max|avg|count)\\((.*)\\)", Qt::CaseInsensitive);
    reField.setMinimal(true);
    int pos = 0;
    while ((pos = reField.indexIn(str, pos)) != -1)
    {
	list.append(reField.cap(0));
	pos += reField.matchedLength();
    }
    return list;
}

void ItemInterface::storeAgregateValuesFromString(QString str)
{
    QStringList functionList = getAgregateFunctionsFromString(str);
    foreach (QString arg1, functionList)
    {
	QRegExp reField("\\((.*)\\)");
	int pos = reField.indexIn(str);
	if (pos > -1)
	    addAgregateValue(reField.cap(1).section(",",0,0).trimmed());
    }
}

QString ItemInterface::calculateAgregateFunctions(QString str, ItemInterface* item)
{
    QString newStr = str;
    QRegExp reField("(sum|min|max|avg|count)\\((.*)\\)", Qt::CaseInsensitive);
    reField.setMinimal(true);
    int pos = 0;

    while ((pos = reField.indexIn(newStr, pos)) != -1)
    {
	QString value = reField.cap(0);
	QString func = reField.cap(1).toLower().trimmed();
	QString args = reField.cap(2);
	qreal result = 0.0;

	if (func == "sum")
	{
	    QString arg = args.contains(",") ? args.section(",",0,0) : args.trimmed(); //first argument
	    foreach (qreal val, item->agregateValues(arg))
		result += val;
	};

	if (func == "avg")
	{
	    QString arg = args.contains(",") ? args.section(",",0,0) : args.trimmed(); //first argument
	    foreach (qreal val, item->agregateValues(arg))
		result += val;
	    result = result / (qreal)item->agregateCounter();
	};
	if (func == "min")
	{
	    QString arg = args.contains(",") ? args.section(",",0,0) : args.trimmed(); //first argument
	    result = INT_MAX;
	    foreach (qreal val, item->agregateValues(arg))
		result = qMin(result,val);
	};
	if (func == "max")
	{
	    QString arg = args.contains(",") ? args.section(",",0,0) : args.trimmed(); //first argument
	    foreach (qreal val, item->agregateValues(arg))
		result = qMax(result,val);
	};
	if (func == "count")
		result = item->agregateCounter();


	newStr.replace(value, QString::number(result));
	pos += reField.matchedLength();
    }
    return newStr;
}

void ItemInterface::addAgregateValue(QString value)
{
    if (dynamic_cast<Report::BandInterface*>(parentItem()) && !value.isEmpty())
	dynamic_cast<Report::BandInterface*>(parentItem())->addAgregateValue(value);
}

QList<qreal> ItemInterface::agregateValues(QString value)
{
    if (dynamic_cast<Report::BandInterface*>(parentItem()))
	return dynamic_cast<Report::BandInterface*>(parentItem())->agregateValues(value);
    else
	return QList<qreal>();
}

int ItemInterface::agregateCounter()
{
     if (dynamic_cast<Report::BandInterface*>(parentItem()))
	return dynamic_cast<Report::BandInterface*>(parentItem())->agregateCounter();
    else
	return 0;
}
