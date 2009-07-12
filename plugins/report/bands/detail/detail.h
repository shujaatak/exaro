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

#ifndef DETAIL_H
#define DETAIL_H

#include <bandinterface.h>

struct PrintParam
{
    QPainter * painter;
    QPointF translate;
    QRectF clipRect;
    int lineNum;
};

struct CashedData
{
    int dsFirst, dsLast;
    QList <PrintParam> param;
};

class Detail : public Report::BandInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);
	Q_ENUMS(ColumnAlignType);

	Q_PROPERTY(QString dataset READ dataset WRITE setDataset);
	Q_PROPERTY(QString datasetFilter READ datasetFilter WRITE setDatasetFilter);
	Q_PROPERTY(int datasetFilterColumn READ datasetFilterColumn WRITE setDatasetFilterColumn);
	Q_PROPERTY(bool zebra READ isZebra WRITE setZebra);
	Q_PROPERTY(QString joinTo READ joinTo WRITE setJoinTo);
	Q_PROPERTY(int columns READ numColumns WRITE setNumColumns);
	Q_PROPERTY(ColumnAlignType columnAlignType READ columnAlignType WRITE setColumnAlignType);

public:
    enum ColumnAlignType {caHorizontal = 1, caVertical = 2};

public:
	Detail(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	~Detail();

	int layoutPriority(){ return 50;}

	bool prInit(Report::PaintInterface * paintInterface);
	bool prData();
	bool prReset();
	bool prPaint(QPainter * painter, QPointF translate, const QRectF & clipRect);

	QRectF boundingRect() const;
	void _paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QRectF & rect,  QWidget * widget = 0);
	bool canContain(QObject * object);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	bool isZebra();			    //light and dark rows
	void setZebra(bool b);

	QString joinTo();
	void setJoinTo(QString bandName);
	QString datasetFilter();
	void setDatasetFilter(QString filter);
	int datasetFilterColumn();			// count 1...n, 0 - not defined, -1 for all columns
	void setDatasetFilterColumn(int column);
	int numColumns();
	void setNumColumns(int col);
	ColumnAlignType columnAlignType();
	void setColumnAlignType (ColumnAlignType align);
	void paintVerticalColumns();

private slots:
	void joinToSlot(QObject * item);
	void checkCurrentBand(BandInterface* band);
	void closePageBefore();
private:
	bool m_isZebra;
	bool darkRow;	//counter for zebra
	QString m_joinTo;
	QString m_datasetFilter;
	int m_datasetFilterColumn;
	int m_numColumns;
	int m_currentColumn;
	ColumnAlignType m_columnAlignType;
	bool m_isFakePass;		    // 2 passes for vertical columns. first = fake pass
	CashedData cashedData;
	int m_lastPage;
};

#endif
