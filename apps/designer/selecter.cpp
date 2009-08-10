/***************************************************************************
 *   This file is part of the eXaro project                                *
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

#include "selecter.h"
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include "bandinterface.h"
#include "mainwindow.h"

#define ZVALUE 100.0

Selecter::Selecter(QGraphicsScene * scene, mainWindow * mw)
	: QObject (scene)
	,m_activeObject(0)
	,m_mw(mw)
	,m_guide(0)
	,needRectRefresh(false)
{
}

Selecter::~Selecter()
{
}

QObject * Selecter::activeObject()
{
	return m_activeObject;
}


QPointF Selecter::activeObjectLastPressPos()
{
    return m_activeObjectPressPos;
}


QObject * Selecter::itemSelected(QObject * object, QPointF pos, Qt::KeyboardModifiers keys)
{
//    qDebug("selected object = %s", qPrintable(object->objectName()));
    m_activeObject = object;
    m_activeObjectPressPos = pos;

    Report::ItemInterface* item = dynamic_cast<Report::ItemInterface*>( object );
    if (!item)
    {
	free();
	return m_activeObject;
    }

    return _itemSelected(item, pos, keys);
}


QObject * Selecter::itemSelected(Report::ItemInterface * item, QPointF pos, Qt::KeyboardModifiers keys )
{
    m_activeObject = static_cast<QObject *>(item);
    m_activeObjectPressPos = pos;

    return _itemSelected(item, pos, keys);
}


QObject * Selecter::_itemSelected(Report::ItemInterface * item, QPointF pos, Qt::KeyboardModifiers keys )
{

    foreach (Item i, items)
	if (i.item == item )
	{
	    if (keys == Qt::ShiftModifier)
	    {
		remove(item);
		m_activeObject = static_cast<QObject *>(items.last().item);
		return items.last().item;
	    }
	    else
	    {
		setGuideItem(item);
		m_activeObject = static_cast<QObject *>(item);
		return item;
	    }
	}

    if (keys != Qt::ShiftModifier)
	free();

    // we can't group Band with any other item
    if (items.count() == 1 && dynamic_cast<Report::BandInterface *>(items[0].item) && keys == Qt::ShiftModifier)
	return items[0].item;

	append(item);
}

void Selecter::append(Report::ItemInterface * item)
{
//    qDebug("Selecter::add");

    // we can't do group operations for Bands now
    if ( dynamic_cast<Report::BandInterface *> (item))
    {
	free();
//	item->setFlag (QGraphicsItem::ItemIsMovable, false);
    }
//    else
//    	item->setFlag (QGraphicsItem::ItemIsMovable, true);

    Item i;
    i.item = item;
    i.pos = item->pos();
    i.sel = new ItemSelection();
    item->scene()->addItem(i.sel);
    i.sel->setItem(item);
    i.sel->setZValue( ZVALUE + 1); //on top of selector's group item

    connect (item, SIGNAL(geometryChanged(QObject*, QRectF)), this, SLOT(itemGeometryChanged(QObject*, QRectF) ) );

    items.append(i);

    QPointF pos = item->mapToScene( item->pos() );
    setGuideItem(items.last().item);
    needRectRefresh = true;
}



void Selecter::remove (Report::ItemInterface * item)
{
    for (int i=0; i< items.count(); i++)
	if (items.at(i).item == item)
	{
	    delete items.at(i).sel;

	    items.at(i).item->setSelected( false );

	    items.removeAt(i);
	    break;
	}
    setGuideItem(items.last().item);
    needRectRefresh = true;
}

void Selecter::free()
{
    if (!items.count())
	return;

    for (int i=0; i< items.count(); i++)
    {
	delete items.at(i).sel;

	items.at(i).item->setSelected( false );
    }

    items.clear();
    needRectRefresh = true;
}

void Selecter::store()
{
    storedItems = items;
    free();
}

void Selecter::restore()
{
    foreach (Item i, storedItems)
	append(i.item);
    storedItems.clear();
}

void Selecter::setGuideItem(Report::ItemInterface * item)
{
    if (!item)
	return;

    if (m_guide)
	disconnect ( m_guide , SIGNAL (geometryChanged(QObject*,QRectF)), this, SLOT (guideGeometryChanged(QObject*,QRectF)) );

    connect ( item , SIGNAL (geometryChanged(QObject*,QRectF)), this, SLOT (guideGeometryChanged(QObject*,QRectF)) );

    m_guideLastPos = item->pos();
    m_guide = item;
}

void Selecter::itemGeometryChanged(QObject* item, QRectF)
{
    for (int i=0; i< items.count(); i++)
	if (items.at(i).item == item)
	{
	items.at(i).sel->updateGeometry();
	break;
    }
    needRectRefresh = true;
}

void Selecter::guideGeometryChanged(QObject* item, QRectF rect)
{
    QPointF dp = rect.topLeft() - m_guideLastPos;
    for (int i=0; i< items.count(); i++)
	if (items.at(i).item != item)
	{
	    items.at(i).item->setPos( items.at(i).item->pos() + dp );
	    items.at(i).sel->updateGeometry();
	}
    m_guideLastPos = rect.topLeft();
    needRectRefresh = true;
}


QList<Report::ItemInterface *> Selecter::selectedItems()
{
    QList<Report::ItemInterface *> list;
    foreach (Item i, items)
	list.append( i.item );

    return list;
}

bool Selecter::haveSelection()
{
    return (bool)items.count();
}

void Selecter::reset()
{
    free();
    m_activeObject = 0;
}

void Selecter::updateSelection()
{
    for (int i=0; i< items.count(); i++)
	items.at(i).sel->updateGeometry();
}

QRectF Selecter::_rect()
{
    needRectRefresh = false;
    if (!items.count())
	return QRect();

    m_boundingRect = items.at(0).item->geometry();
    if (items.count() > 1)
    for (int i=1; i< items.count(); i++)
	m_boundingRect = m_boundingRect.united( items.at(i).item->geometry());

    return m_boundingRect;
}

QRectF Selecter::rect()
{
    if (!needRectRefresh)
	return m_boundingRect;
    return _rect();
}

QPointF Selecter::pos()
{
    return rect().topLeft();
}

void Selecter::setPos(QPointF pos)
{
    if (m_guide)
	disconnect ( m_guide , SIGNAL (geometryChanged(QObject*,QRectF)), this, SLOT (guideGeometryChanged(QObject*,QRectF)) );

    QPointF dp = pos - this->pos();
     foreach (Item i, items)
	i.item->setPosition( i.item->pos() + dp );

     if (m_guide)
	 connect ( m_guide , SIGNAL (geometryChanged(QObject*,QRectF)), this, SLOT (guideGeometryChanged(QObject*,QRectF)) );
//     m_boundingRect.setTopLeft( m_boundingRect.topLeft() + dp );
	 needRectRefresh = true;
}
