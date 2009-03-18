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

#ifndef ABSTRACTREPORTITEM_H
#define ABSTRACTREPORTITEM_H
#include <QGraphicsItem>
#include <QObject>
#include <QIcon>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QScriptEngine>

#include "globals.h"
#include "paintinterface.h"

/** @namespace Report */

namespace Report
{

class SqlQuery;
class ReportInterface;
class PaintInterface;

class KONTAMABIL_EXPORTS ItemInterface: public QObject, public QGraphicsItem
{
	Q_OBJECT

	Q_FLAGS(Frames Frame);

	/**
	 * @see isEnabled()
	 * @see setEnabled()
	*/
	Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
	/**
	 * @see geometry()
	 * @see setGeometry()
	*/
	Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry)
	/**
	 * @see opacity()
	 * @see setOpacity()
	*/
	Q_PROPERTY(int opacity READ opacity WRITE setOpacity)
	/**
	 * @see frame()
	 * @see setFrame()
	*/
	Q_PROPERTY(Frames frame READ frame WRITE setFrame)

public:
	/**
	* Frame enum
	*
	* @see frame()
	* @see setFrame()
	*/
	enum Frame {DrawLeft = 1, /**< Draw left frame*/
		    DrawRight = 2, /**< Draw right frame*/
		    DrawTop = 4, /**< Draw top frame*/
		    DrawBottom = 8 /**< Draw bottom frame*/
		   };
	/** @enum ResizeFlags
	* @see resizeFlags()
	* @see setResizeFlags()
	*/
	enum ResizeFlags {Fixed = 0, /**< Item cant be resized*/
	                  ResizeLeft = 1, /**< Item can be resized to left*/
	                  ResizeRight = 2, /**< Item can be resized to right*/
	                  ResizeTop = 4, /**< Item can be resized to top*/
	                  ResizeBottom = 8, /**< Item can be resized to bottom*/
	                  FixedPos = 16 /**< Item cant be moved*/
	                 };

	Q_DECLARE_FLAGS(Frames, Frame);

public:
	/**
	 * ItemInterface constructor
	 * @param parent parent item
	 * @param parentObject parent object
	 */
	ItemInterface(QGraphicsItem* parent = 0, QObject * parentObject = 0);
	virtual ~ItemInterface();

	/**
	* Return what frame should draw
	* @return BandInterface::Frames
	* @see setFrame()
	* @see Frame
	*/
	Frames frame();
	/**
	* Set the band frames
	* @param frame
	* @see frame()
	* @see Frame
	*/
	void setFrame(Frames frame);
	/**
	 * Return resizeHandle pixels
	 * @return pixels
	 * @see setResizeHandle()
	 */
	int resizeHandle();
	/**
	 * Set resizeHandle pixels (default 6)
	 * @param resizeHandle pixels
	 * @see resizeHandle()
	 */
	void setResizeHandle(int resizeHandle);

	/**
	 * Return the item width
	 * @return item width
	 * @see setWidth();
	 */
	virtual qreal width() const;
	/**
	 * Set the item width
	 * @param width
	 * @see width();
	 */
	virtual void setWidth(qreal width);

	/**
	 * Return the item height
	 * @return item height
	 * @see setHeight()
	 */
	virtual qreal height() const;
	/**
	 * Set the item height
	 * @param height item height
	 * @see height()
	 */
	virtual void setHeight(qreal height);
	/**
	* Returns the band stretch.
	* @return int
	*/
	qreal stretch();
	/**
	* Set the band stretch.
	* @param val stretch value
	*/
	void setStretch(qreal val);
	/**
	* Set the band stretch to 0.
	*/
	void unstretch();

	/**
	 * Return the item minimal width
	 * @return item min width
	 * @see setMinWidth()
	 */
	virtual qreal minWidth() const;
	/**
	 * Return the item minimal height
	 * @return item min height
	 * @see setMinHeight()
	 */
	virtual qreal minHeight() const;
	/**
	 * Set the item minimum height
	 * @param height item minimum height
	 * @see minHeight()
	 */
	virtual void setMinHeight(qreal height);
	/**
	 * Set the item minimum width
	 * @param width item width
	 * @see minWidth()
	 */
	virtual void setMinWidth(qreal width);

	/**
	 * Return resize flags
	 * @return resize flags
	 * @see ResizeFlags
	 */
	int resizeFlags();

	/**
	 * Return resize flags for a given point
	 * @param cursor cursor point
	 * @return resize flags
	 */
	int posibleResizeCurrsor(QPointF cursor);

	/**
	 * Return the item geometry
	 * @return item geometry
	 */
	virtual QRectF geometry();
	/**
	 * Set the item geometry
	 * @param rect item geometry
	 */
	virtual void setGeometry(QRectF rect);

	/**
	 * Return the icon for tool box
	 * @return icon
	 */
	virtual QIcon toolBoxIcon()
	{
		return QIcon();
	};

	/**
	 * Return the item name for tool box
	 * @return item name
	 */
	virtual QString toolBoxText() = 0;
	/**
	 * Return the item group
	 * @return item group
	 */
	virtual QString toolBoxGroup() = 0;

	/**
	 * This function defines the outer bounds of the item as a rectangle; all painting must be restricted to inside an item's bounding rect.
	 * @return bounding rect
	 */
	QRectF boundingRect() const
	{
		return QRectF();
	};

	/**
	 * This function prepare the item. This function is called before paint function
	 * @param painter the painter used to paint in
	 */
	//virtual bool prepare(QPainter * painter, PaintInterface::PrintMode pMode = PaintInterface::pmNormal);
	/**
	 * This function paints the contents of an item
	 * @param painter the painter used to paint in
	 * @param option provides style options for the item, such as its state, exposed area and its level-of-detail hints.
	 * @param widget the widget argument is optional. If provided, it points to the widget that is being painted on; otherwise, it is 0. For cached painting, widget is always 0.
	 */
	virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	/**
	 * This pure virtual function is used to create the item object
	 * @param parent parent item
	 * @param parentObj parent object
	 * @return item instance
	 */
	virtual QObject * createInstance(QGraphicsItem* parent = 0, QObject * parentObj = 0) = 0;

	/**
	 * Before you add a object to this item, call this method to see if this item can containt the object.
	 * @param object object you want to add
	 * @return true if the report can contain the object
	 */
	virtual bool canContain(QObject * object);

	/**
	 * Try to find the report query query
	 * @param query query to search
	 * @return a valid SqlQuery query pointer or 0.
	 */
	SqlQuery * findQuery(const QString & query);

	/**
	 * Return the field from query
	 * @param query query to find
	 * @param field field to find
	 * @return an QVariant value or an error message
	 */
	QVariant queryField(const QString & query, const QString & field);

	/**
	 * Return the report script engine
	 * @return script engine
	 */
	QScriptEngine * scriptEngine();

	/**
	 * Returns the report object of the item
	 * @return report object
	 */
	ReportInterface	* reportObject();

	/**
	* Return the item opacity
	* @return item opacity
	* @see setOpacity()
	*/
	virtual int opacity();
	/**
	 * Set the item opacity
	 * @param opacity item opacity
	 * @see opacity()
	 */
	virtual void setOpacity(const int opacity);

	/**
	 * Return the parent geometry
	 * @return parent geometry
	 */
	QRectF parentGeometry();

	virtual bool isEnabled();
	virtual void setEnabled(bool b);

	void raise();

	virtual bool init(PaintInterface * paintInterface);	    //default implementation do nothing
//	virtual bool prePaint();	    //default implementation do nothing
	virtual bool prePaint(QPainter * painter, PaintInterface::PrintMode pMode = PaintInterface::pmNormal);
	virtual bool postPaint();	    //default implementation do nothing

	virtual QString lastError();

	QString setExpressionDelimeters(QString str);	    // expression delimeter for exec scripts in text
	QString expressionDelimeters();
	QString processString(QString str);
	static bool stringIsField(QString str);
	static bool stringToField (QString str, QString * query, QString * field);
	static QStringList getAgregateFunctionsFromString(QString str);
	static QString calculateAgregateFunctions(QString str, ItemInterface* item);
	void storeAgregateValuesFromString(QString str);
	virtual void addAgregateValue(QString value);
	virtual QList<qreal> agregateValues(QString value);
	virtual int agregateCounter();

protected:
	/**
	 * This event handler, for event event, can be reimplemented to receive mouse press events for this item. Mouse press events are only delivered to items that accept the mouse button that is pressed. By default, an item accepts all mouse buttons, but you can change this by calling setAcceptedMouseButtons().
	 * @param event QGraphicsSceneMouseEvent
	 */
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	/**
	 * This event handler, for event event, can be reimplemented to receive mouse release events for this item.
	 * @param event QGraphicsSceneMouseEvent
	 */
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	/**
	 * This event handler, for event event, can be reimplemented to receive mouse move events for this item. If you do receive this event, you can be certain that this item also received a mouse press event, and that this item is the current mouse grabber.
	 * @param event QGraphicsSceneMouseEvent
	 */
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

	/**
	 * Set the resizeFlags for the item. This should set only once in the item constructor.
	 * @param resizeFlags
	 * @see ResizeFlags
	 */
	void setResizeFlags(int resizeFlags);

	/**
	 * This function is used to draw the selection of the item
	 * @param painter the painter to draw to
	 * @param rect bounding rect
	 */
	void drawSelection(QPainter * painter, QRectF rect);

	/**
	 * This function set the background brush, the brush, the pen and the font to painter
	 * @param painter
	 */
	virtual void setupPainter(QPainter * painter);

	static QFont fontConvert(QFont & font);
	static const QRectF adjustRect(QRectF & rect, const QPen & pen);

signals:
	/**
	 * This signal is emitted when the item is selected
	 * @param thisObj item object
	 * @param cursorPos cursor position
	 */
	void itemSelected(QObject * thisObject, QPointF cursorPos);

	/**
	 * This signal is emitted when the item geometry is changed
	 * @param newGeometry new geometry
	 */
	void geometryChanged(QRectF newGeometry);

	/**
	 * This signal should be emitted before printing
	 * @param sender object how emitt the signal
	 */
	void beforePrint(QObject * sender);

	/**
	 * This signal should be emitted after printing
	 * @param sender object how emitt the signal
	 */
	void afterPrint(QObject * sender);
	
	void geometryChanged(QObject * object, QRectF newGeometry, QRectF oldGeometry);
protected:
	QString m_lastError;
	PaintInterface * m_paintInterface;

private:
	Frames m_frame;
	int m_resizeHandle;
	int m_resizeEvent;
	int m_resizeFlags;
	qreal m_width, m_height;
 	qreal m_minWidth, m_minHeight;
	int m_opacity;
	qreal m_stretch;
	QRectF oldGeometry;
	bool m_drawSelectionBorder;
	bool m_enabled;
	QString expBegin, expEnd;	    //expression delimeter for check and execute scripts in text
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ItemInterface::Frames);
}

Q_DECLARE_INTERFACE(Report::ItemInterface, "ro.bigendian.Report.ItemInterface/1.0");
#endif
