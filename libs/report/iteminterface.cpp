/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 *   Copyright (C) 2009 by Mikhalov Alexaner                               *
 *   alexmi3@rambler.ru                                                    *
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
#include <QStyleOptionGraphicsItem>

#include "iteminterface.h"
#include "bandinterface.h"
#include "reportinterface.h"
#include "pageinterface.h"
#include "dataset.h"
#include "itemhelper.h"

#define CORNERSIZE 20

using namespace Report;

ItemInterface::ItemInterface(QGraphicsItem* parent, QObject * parentObject): QObject(parentObject), QGraphicsItem(parent)
		, m_minWidth(5)
		, m_minHeight(5)
		, m_stretch(0)
		, m_paintInterface(0)
		, offsetX(0), offsetY(0), _offsetX(0), _offsetY(0)
{
    	m_frame = 0;
//	m_resizeEvent = Fixed;
	m_resizeFlags = ResizeTop | ResizeBottom | ResizeLeft | ResizeRight;
	m_width = 20/UNIT; // 20 mm
	m_height = 20/UNIT; // 20 mm
	m_opacity = 100;
	m_rotate = 0;
	m_enabled = true;
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemClipsChildrenToShape);
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

int ItemInterface::resizeFlags()
{
	return m_resizeFlags;
}

void ItemInterface::setResizeFlags(int resizeFlags)
{
	m_resizeFlags = resizeFlags;
}


void ItemInterface::selectItem(QPointF pos, Qt::KeyboardModifiers keys)
{
    emit(itemSelected(this, pos, keys));
    raise();
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

    /// corners
    if (option->type == QStyleOption::SO_GraphicsItem)
    {
	painter->setPen(QColor(0,0,0,100));
	painter->drawLine(0,0,0,CORNERSIZE);
	painter->drawLine(0,0,CORNERSIZE,0);
	painter->drawLine(rect.width(),0,rect.width()-CORNERSIZE,0);
	painter->drawLine(rect.width(),0,rect.width(),CORNERSIZE);
	painter->drawLine(rect.width(),rect.height(),rect.width()-CORNERSIZE, rect.height());
	painter->drawLine(rect.width(),rect.height(),rect.width(), rect.height()-CORNERSIZE);
	painter->drawLine(0,rect.height(), CORNERSIZE, rect.height());
	painter->drawLine(0,rect.height(), 0, rect.height()-CORNERSIZE);
    }

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

void ItemInterface::setWidth(qreal width)
{
	prepareGeometryChange();
	m_width = width;
	emit(geometryChanged(geometry()));
	emit(geometryChanged(this, geometry()));
}

void ItemInterface::setHeight(qreal height)
{
	prepareGeometryChange();
	m_height = height;
	emit(geometryChanged(geometry()));
	emit(geometryChanged(this, geometry()));
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

}

void ItemInterface::setMinWidth(qreal width)
{
    width > 0 ? m_minWidth = width: 0;

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
		return tr("Dataset '%1' not found").arg(dataset);

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
    emit beforePrint(this);

    QStyleOptionGraphicsItem option;
    option.type = 31;
    painter->save();
    painter->resetMatrix();
    painter->translate(translate);
    option.exposedRect = dynamic_cast<BandInterface *>(this) ? QRectF(0, 0, geometry().width(), geometry().height()) : geometry();
    translate += option.exposedRect.topLeft() + QPointF(offsetX + _offsetX, offsetY + _offsetY);
//    painter->setClipRect(clipRect);

    if (m_rotate)
    {
	QPointF p = option.exposedRect.center();
	painter->translate(p.x(), p.y());
	painter->rotate( m_rotate );
	painter->translate( -p.x(), -p.y() );
    }

    paint(painter, &option);
    painter->restore();

    foreach(QObject * child, QObject::children())
    {
    if (ItemInterface * childItem = dynamic_cast<ItemInterface *>(child))
	if (childItem->prData())
	    childItem->prPaint(painter, translate, option.exposedRect);
    }
    emit afterPrint(this);
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
    if (str.contains(expBegin))
    {
	QString cleanPart = str.section(expBegin,0,0);
	QString secondPart = str.section(expBegin,1);
	QString expression = secondPart.section(expEnd,0,0);

	QString uncheckedPart = secondPart.section(expEnd,1);

	#warning 'TODO: emplement expression validation'
	expression = calculateAgregateFunctions(expression, this); // only expression func(ds."field";arg) is valid

	QRegExp reField("\\w+\\b*\\.{1}\\\".*\\\"");
	reField.setMinimal(true);

	bool needEvaluate = false;
	bool haveField = false;
	int pos = 0;
	while ((pos = reField.indexIn(expression, pos)) != -1)
	{
	    // workaround to prevent calculate string dataset field such as "1998-03-02". It's shown as 1993.
	    haveField = true;
	    if (reField.cap(0) != expression)
		needEvaluate = true;

	    QString dataset = reField.cap(0).section(".",0,0);
	    QString field = reField.cap(0).section(".",1,1).remove("\"");
	    expression.replace(reField.cap(0), datasetField(dataset,field).toString());
	    pos += reField.matchedLength();
	}
	if (!haveField)
	   needEvaluate = true;
	qDebug("string = \'%s\'     evaluate = %i", qPrintable(expression), (int)needEvaluate);

	if (needEvaluate)
	{
	    QString evaluateStr;
	    #warning 'FIXMI: need optimize checking for script present'
	    if (scriptEngine()->canEvaluate(expression))
		    evaluateStr = scriptEngine()->evaluate(expression).toString();
	    if (!scriptEngine()->hasUncaughtException())
		expression = evaluateStr;
	    //	else qWarning("QtScript has exeption in:\n%s",qPrintable(evaluateStr));
	}
	str = cleanPart + expression + processString(uncheckedPart);
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
	    QString arg = args.contains(";") ? args.section(";",0,0) : args.trimmed(); //first argument
	    foreach (qreal val, item->agregateValues(arg))
		result += val;
	};

	if (func == "avg")
	{
	    QString arg = args.contains(";") ? args.section(";",0,0) : args.trimmed(); //first argument
	    foreach (qreal val, item->agregateValues(arg))
		result += val;
	    result = result / (qreal)item->agregateCounter();
	};
	if (func == "min")
	{
	    QString arg = args.contains(";") ? args.section(";",0,0) : args.trimmed(); //first argument
	    result = INT_MAX;
	    foreach (qreal val, item->agregateValues(arg))
		result = qMin(result,val);
	};
	if (func == "max")
	{
	    QString arg = args.contains(";") ? args.section(";",0,0) : args.trimmed(); //first argument
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

void ItemInterface::setPosition(qreal x, qreal y)
{
    setPos(x,y);
    emit geometryChanged(this, geometry());
}

void ItemInterface::setPosition(QPointF pos)
{
    setPos(pos);
    emit geometryChanged(this, geometry());
}

qreal ItemInterface::rotate()
{
    return m_rotate;
}

void ItemInterface::setRotate(qreal angle)
{
    m_rotate = angle;

    QPointF p = boundingRect().center();
    setTransform(QTransform().translate(p.x(), p.y()).rotate(m_rotate).translate( -p.x(), -p.y()));

    update();

    emit geometryChanged(this, geometry());
}

ItemHelper * ItemInterface::createHelper()
{
    return 0;
}
