#include "selecter.h"
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include "bandinterface.h"

#define ZVALUE 100.0

Selecter::Selecter(QGraphicsScene * scene)
	: QObject (scene)
	,m_activeObject(0)
{
    sItem = new GraphicsItemGroup();
    scene->addItem(sItem);
    sItem->setZValue(ZVALUE);
    sItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    sItem->setSelected( true );
}

Selecter::~Selecter()
{
//    free();
//    delete sItem;
}

QObject * Selecter::activeObject()
{
//    qDebug("count = %i", items.count());
    if (items.count())
	return /*dynamic_cast<QObject *>*/(items.last().item);
    else
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
	return m_activeObject;

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

    if (item->scene() == sItem->scene())
	add(item);
}

void Selecter::itemMoved(Report::ItemInterface * item, QPointF oldPos)
{
    qDebug("group moved");
}

void Selecter::add (Report::ItemInterface * item)
{
    qDebug("Selecter::add");

    // we can't do group operations for Bands now
    if ( dynamic_cast<Report::BandInterface *> (item))
    {
	free();
	sItem->setFlags (sItem->flags() ^ QGraphicsItem::ItemIsMovable);
    }
    else
	sItem->setFlags (sItem->flags() | QGraphicsItem::ItemIsMovable);

    Item i;
    i.item = item;
    i.pos = item->pos();
    i.parent = dynamic_cast<Report::ItemInterface *> (item->parentItem());
    i.sel = new ItemSelection();
    item->scene()->addItem(i.sel);
    i.sel->setItem(item);
    i.sel->setZValue( ZVALUE + 1); //on top of selector's group item

    items.append(i);

    // workaround - addToGroup don't work without this, and i don't know why
    QPointF iPos = item->pos();
    item->setParentItem(0);
    if (i.parent)
	item->setPos( i.parent->mapToScene ( iPos ) );
    // ---

    sItem->addToGroup( item );

    QPointF pos = item->mapToScene( item->pos() );
    setGuideItem(items.last().item);

}

void Selecter::remove (Report::ItemInterface * item)
{
    qDebug("Selecter::remove");
    for (int i=0; i< items.count(); i++)
	if (items.at(i).item == item)
	{
	    sItem->removeFromGroup( items.at(i).item );
	    delete items.at(i).sel;

	    if (items.at(i).parent)
	    {
		items.at(i).item->setParentItem(items.at(i).parent);
		items.at(i).item->setPos( items.at(i).parent->mapFromScene(items.at(i).item->pos()));
		items.at(i).item->setZValue( items.at(i).zValue );
		items.at(i).item->setSelected( false );
	    }

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
	sItem->removeFromGroup( items.at(i).item );
	delete items.at(i).sel;

	if (items.at(i).parent)
	{
	    items.at(i).item->setParentItem(items.at(i).parent);
	    items.at(i).item->setPos( items.at(i).parent->mapFromScene(items.at(i).item->pos()));
	    items.at(i).item->setZValue( items.at(i).zValue );
	    items.at(i).item->setSelected( false );
	}
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
	add(i.item);
    storedItems.clear();
}

void Selecter::setGuideItem(Report::ItemInterface * item)
{
//    qDebug("set Guide = %s", qPrintable(item->objectName()) );
    foreach (Item i, items)
	i.sel->setGuideItem(item);
}
