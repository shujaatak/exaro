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

#ifndef BANDINTERFACE_H
#define BANDINTERFACE_H

#include <QObject>
#include <QGraphicsItem>
#include "iteminterface.h"
#include "globals.h"
#include "bandtitle.h"

/** \namespace Report */
namespace Report
{

struct ValueStruct
{
    QString dataset;	//if empty - use QtScript values
    QString field;
    QList<qreal> list;
};


class BandTitle;

/** \class BandInterface
* \brief Interface for bands
*
* This is the base class for all bands objects.
*/
class KONTAMABIL_EXPORTS BandInterface : public ItemInterface
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
	Q_PROPERTY(int indentationTop READ indentationTop WRITE setIndentationTop)
	Q_PROPERTY(int indentationBottom READ indentationBottom WRITE setIndentationBottom)

//	Q_PROPERTY(QString joinTo READ joinTo WRITE setJoinTo)
public:
	/**
	* TitlePosition enum
	*
	* @see drawTitle()
	*/
	enum TitlePosition {TitleLeft, /**< Draw the title on left side*/
			    TitleRight /**< Draw the title on right side*/
			    ,TitleTop
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

	virtual bool prNewPage();
	virtual bool prClosePage();

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
	int indentationTop();
	int indentationBottom();
	/**
	* Set the band indentation. The indentation is the space between bands.
	* Can be negative for crossed bands
	* @param indentation
	*/
	void setIndentationTop(int indentation);
	void setIndentationBottom(int indentation);

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

	int freeSpace();
	void addAgregateValue(QString value);
	QList<qreal> agregateValues(QString value);
	void accumulateAgregateValues();
	void resetAgregateValues();
	int agregateCounter();

//	virtual QString joinTo();			// return band to which current is joined
//	virtual void setJoinTo(QString bandName);	// can be useful in some cases inside band
//	static BandInterface::TitlePosition titlePosition();
//	static void setTitlePosition (BandInterface::TitlePosition pos);
//	Report::BandTitle * title();
	void showTitle(bool b = true);
	void hideTitle();
	QRectF titleGeometry();
	static bool hasTitle();
	static void setTitleEnabled(bool b);

signals:
	void bandDelete(int);

protected:
	virtual void createTitle(const QString & title);
	QVariant itemChange ( GraphicsItemChange change, const QVariant & value );

protected slots:
	/**
	 * This slot is called when parent geometry is changed
	 * @param rect parent geometry
	 * @see setGeometry()
	 */
	void updateGeometry(QRectF rect);
	void setTitleGeometry(QRectF rect);

private:
	int m_order;
	int m_indentationTop, m_indentationBottom;
	bool m_deleting;
	Report::BandTitle * m_titleItem;

protected:
	QHash <QString, ValueStruct> m_agregateValues;
	int m_agregateCounter;
	QString m_dataset;
};
}
Q_DECLARE_INTERFACE(Report::BandInterface, "ReportDesigner.BandInterface/1.0");

#endif
