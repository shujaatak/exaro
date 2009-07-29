#include "selecter.h"
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include "bandinterface.h"

#define ZVALUE 100.0

Selecter::Selecter(QGraphicsScene * scene)
{
    sItem = new QGraphicsItemGroup();
    scene->addItem(sItem);
    sItem->setZValue(ZVALUE);
    sItem->show();
    sItem->update();
}

Selecter::~Selecter()
{
    free();
}

void Selecter::add (Report::ItemInterface * item)
{
    qDebug("Selecter::add");

    /// we can't do group operations for Bands now
    if ( dynamic_cast<Report::BandInterface *> (item))
	free();

    Item _item;
    _item.item = item;
    _item.parent = dynamic_cast<Report::ItemInterface *> (item->parentItem());
    _item.sel = new ItemSelection();
    item->scene()->addItem(_item.sel);
    _item.sel->setItem(item);
    _item.sel->setZValue( ZVALUE + 1); //on top of selector's group item

    items.append(_item);

    /// workaround - addToGroup don't work without this, and i don't know why
    QPointF iPos = item->pos();
    item->setParentItem(0);
    if (_item.parent)
	item->setPos( _item.parent->mapToScene ( iPos ) );
    /// --------------------------------------------------------------------

    sItem->addToGroup( item );

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
	}
    }

    items.clear();
}
