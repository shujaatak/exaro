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

#ifndef BANDINTERFACE_H
#define BANDINTERFACE_H

#include <QObject>
#include <QGraphicsItem>
#include "iteminterfaceext.h"
#include "globals.h"

/** \namespace Report */
namespace Report
{

struct ValueStruct
{
    QString dataset;	//if empty - use QtScript values
    QString field;
    QList<qreal> list;
};


class TitleItem;

/** \class BandInterface
* \brief Interface for bands
*
* This is the base class for all bands objects.
*/
class KONTAMABIL_EXPORTS BandInterface : public ItemInterfaceExt
{
	Q_OBJECT

	/**
	 * @see order()
	 * @see setOrder()
	*/
	Q_PROPERTY(int order READ order WRITE setOrder)
	/**
	 * @see indentation()
	 * @see setIndentation()
	*/
	Q_PROPERTY(int indentation READ indentation WRITE setIndentation)
public:
	/**
	* TitlePosition enum
	*
	* @see drawTitle()
	*/
	enum TitlePosition {TitleLeft, /**< Draw the title on left side*/
			    TitleRight /**< Draw the title on right side*/
			    };
	enum LayoutType { LayoutTop, LayoutBottom, LayoutFree };

	enum AccomodationType {AccomodationOnce, AccomodationFirstPage, AccomodationLastPage, AccomodationEveryPage};


public:
	/**
	* BandInterface constructor
	*
	* @param parent QGraphicsItem parent
	* @param parentObject QObject parent
	*/
	BandInterface(QGraphicsItem* parent = 0, QObject * parentObject = 0);

	~BandInterface();


	virtual LayoutType layoutType();

	virtual int layoutPriority();

	virtual AccomodationType accommodationType();

	/**
	 * Set the band geometry.
	 * @param rect the new geometry. The new geometry will be intersected with the page geometry.
	 */
	void setGeometry(QRectF rect);

	/**
	* Return the current order of the band
	* @return current order
	* @see setOrder()
	*/
	int order();
	/**
	* Set the band order. If you have more bands with the same type you can order them.
	* @param order new order
	* @param refreshOthers if this operation should refresh all bands
	* @see order()
	*/
	void setOrder(int order, bool refreshOthers = true);
	/**
	* Returns the space between bands.
	* @return int
	*/
	int indentation();
	/**
	* Set the band indentation. The indentation is the space between bands.
	* @param indentation
	*/
	void setIndentation(int indentation);

	virtual void setHeight(qreal height);

	/**
	 * Return the query name
	 * @return query name
	 */
	QString dataset();
	/**
	 * Sets the query name
	 * @param query query name
	 */
	void setDataset(const QString & dataset);

	bool deleting();

	void removeItem();

	void selectBand();

	int freeSpace();
	void addAgregateValue(QString value);
	QList<qreal> agregateValues(QString value);
	void accumulateAgregateValues();
	void resetAgregateValues();
	int agregateCounter();

signals:
	void bandDelete(int);

protected:

	/**
	* Draw the title 
	* @param title title text
	* @param position title position
	* @param position text flags
	* @see TitlePosition
	*/
	virtual void drawTitle(const QString & title, TitlePosition position, int textFlags);

protected slots:
	/**
	 * This slot is called when parent geometry is changed
	 * @param rect parent geometry
	 * @see setGeometry()
	 */
	void updateGeometry(QRectF rect);
	//void bandDestroyed(/*int type,*/ int order);

private:
	int m_order;
	int m_indentation;
	QString m_dataset;
	bool m_deleting;
	TitleItem * m_titleItem;

protected:
	QHash <QString, ValueStruct> m_agregateValues;
	int m_agregateCounter;
};
}
Q_DECLARE_INTERFACE(Report::BandInterface, "ro.bigendian.ReportDesigner.BandInterface/1.0");

#endif
