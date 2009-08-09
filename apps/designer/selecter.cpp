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
    qDebug("selected object = %s", qPrintable(object->objectName()));
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

void Selecter::itemMoved(Report::ItemInterface * item, QPointF oldPos)
{
    qDebug("group moved");
}

void Selecter::append(Report::ItemInterface * item)
{
    qDebug("Selecter::add");

    // we can't do group operations for Bands now
    if ( dynamic_cast<Report::BandInterface *> (item))
    {
	free();
	item->setFlag (QGraphicsItem::ItemIsMovable, false);
    }
    else
    	item->setFlag (QGraphicsItem::ItemIsMovable, true);

    qDebug(" is item movable = %i", (int)item->flags().testFlag(QGraphicsItem::ItemIsMovable));

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
}



void Selecter::remove (Report::ItemInterface * item)
{
    qDebug("Selecter::remove");
    for (int i=0; i< items.count(); i++)
	if (items.at(i).item == item)
	{
	    delete items.at(i).sel;

	    items.at(i).item->setSelected( false );

	    items.removeAt(i);
	    break;
	}
    setGuideItem(items.last().item);
}

void Selecter::free()
{
    qDebug("Selecter::free");
    if (!items.count())
	return;

    for (int i=0; i< items.count(); i++)
    {
	delete items.at(i).sel;

	items.at(i).item->setSelected( false );
    }

    items.clear();
}

void Selecter::store()
{
//    qDebug("store");
    storedItems = items;
    free();
}

void Selecter::restore()
{
//    qDebug("restore");
    foreach (Item i, storedItems)
	append(i.item);
    storedItems.clear();
}

void Selecter::setGuideItem(Report::ItemInterface * item)
{
    qDebug("set Guide = %s", qPrintable(item->objectName()) );
    foreach (Item i, items)
	i.sel->setGuideItem(item);
}

void Selecter::itemGeometryChanged(QObject* item, QRectF)
{
    for (int i=0; i< items.count(); i++)
	if (items.at(i).item == item)
	{
	items.at(i).sel->updateGeometry();
	break;
    }
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
