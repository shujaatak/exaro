#include "selecter.h"
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include "bandinterface.h"

#define ZVALUE 100.0

Selecter::Selecter(QGraphicsScene * scene) : QObject (scene)
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


void Selecter::itemSelected(Report::ItemInterface * item, Qt::KeyboardModifiers keys )
{
    // prevent duplicate
    foreach (Item i, items)
	if (i.item == item )
	    return;

    if (keys != Qt::ShiftModifier)
	free();

    // we can't group Band with any other item
    if (items.count() == 1 && dynamic_cast<Report::BandInterface *>(items[0].item) && keys == Qt::ShiftModifier)
	return;

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
    sItem->addToGroup( i.sel);

    QPointF pos = item->mapToScene( item->pos() );
}

void Selecter::remove (Report::ItemInterface * item)
{
    qDebug("Selecter::remove");
    for (int i=0; i< items.count(); i++)
	if (items.at(i).item == item)
	{
	    sItem->removeFromGroup( item );
	    item->setParentItem(items.at(i).parent);
	    items.removeAt(i);
	    break;
	}
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
    storedItems = items;
    free();
}

void Selecter::restore()
{
    foreach (Item i, items)
	add(i.item);
    storedItems.clear();
}
