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

#ifndef ITEMINTERFACEEXT_H
#define ITEMINTERFACEEXT_H

#include <QObject>
#include <QGraphicsItem>
#include "iteminterface.h"
#include "globals.h"

/** \namespace Report */
namespace Report
{

class KONTAMABIL_EXPORTS ItemInterfaceExt : public ItemInterface
{
	Q_OBJECT
    	Q_ENUMS(BGMode)

	/**
	 * @see pen()
	 * @see setPen()
	*/
	Q_PROPERTY(QPen pen READ pen WRITE setPen)
	/**
	 * @see brush()
	 * @see setBrush()
	*/
	Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
	/**
	 * @see backgroundBrush()
	 * @see setBackgroundBrush()
	*/
	Q_PROPERTY(QBrush backgroundBrush READ backgroundBrush WRITE setBackgroundBrush)
	/**
	 * @see backgroundMode()
	 * @see setBackgroundMode()
	 * @see BGMode
	*/
	Q_PROPERTY(BGMode backgroundMode READ backgroundMode WRITE setBackgroundMode)
	/**
	 * @see font()
	 * @see setFont()
	*/
	Q_PROPERTY(QFont font READ font WRITE setFont)

public:
	/** @enum BGMode
	* @see backgroundMode()
	* @see setBackgroundMode()
	*/
	enum BGMode { TransparentMode, /**< Background is transparent*/
		      OpaqueMode /**< Background is opaque*/
		    };

public:
	/**
	* ItemInterfaceExt constructor
	*
	* @param parent QGraphicsItem parent
	* @param parentObject QObject parent
	*/
	ItemInterfaceExt(QGraphicsItem* parent = 0, QObject * parentObject = 0);

	~ItemInterfaceExt();

	/**
	 * Adjust the rect with pen width
	 * @param rect rect to adjust
	 * @return adjusted rect
	 */
	QRectF adjustRect(QRectF & rect);

	/**
	 * Return the brush used to paint background
	 * @return backgroundBrush
	 * @see setBackgroundBrush()
	 */
	virtual QBrush backgroundBrush();
	/**
	 * Set the brush used to paint background
	 * @param brush brush used to paint background
	 * @see backgroundBrush()
	 */
	virtual void setBackgroundBrush(const QBrush & brush);

	/**
	 * Return the brush used to paint the item
	 * @return  the brush used to paint the item
	 * @see setBrush()
	 */
	virtual QBrush brush();
	/**
	 * Set the brush used to paint the item
	 * @param brush brush used to paint the item
	 * @see brush()
	 */
	virtual void setBrush(const QBrush & brush);

	/**
	 * Return the pen used to paint the item
	 * @return pen used to paint the item
	 * @see setPen()
	 */
	virtual QPen pen();
	/**
	 * Set the pen used to paint the item
	 * @param pen
	 * @see pen()
	 */
	virtual void setPen(const QPen & pen);

	/**
	 * Return the item font
	 * @return item font
	 * @see setFont()
	 */
	virtual QFont font();
	/**
	 * Set the item font
	 * @param font item font
	 * @see font()
	 */
	virtual void setFont(const QFont & font);

	/**
	 * Return background mode
	 * @return background mode
	 * @see BGMode
	 * @see setBackgroundMode()
	 */
	virtual BGMode backgroundMode();
	/**
	 * Set background mode
	 * @param bgMode background mode
	 * @see BGMode
	 * @see backgroundMode()
	 */
	virtual void setBackgroundMode(BGMode bgMode);

	/**
	 * This function set the background brush, the brush, the pen and the font to painter
	 * @param painter
	 */
	virtual void setupPainter(QPainter * painter);

    private:
	BGMode m_BGMode;
	QBrush m_brush, m_backgroundBrush;
	QPen m_pen;
	QFont m_font;
};

//Q_DECLARE_OPERATORS_FOR_FLAGS(ItemInterfaceExt::Frames);
}
Q_DECLARE_INTERFACE(Report::ItemInterfaceExt, "ro.bigendian.ReportDesigner.ItemInterfaceExt/1.0");

#endif
