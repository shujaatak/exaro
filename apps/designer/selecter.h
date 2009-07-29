#ifndef SELECTER_H
#define SELECTER_H
#include <QList>
#include "iteminterface.h"
#include "itemselection.h"

class QGraphicsItemGroup;
class QGraphicsScene;

struct Item
{
    Report::ItemInterface * item;
    qreal zValue;
    Report::ItemInterface * parent;
    ItemSelection * sel;
};

class Selecter
{
public:
    Selecter( QGraphicsScene * scene);
    ~Selecter();

    void add (Report::ItemInterface * item);
    void remove (Report::ItemInterface * item);
    void free();

private:
    QGraphicsItemGroup * sItem;
    QList<Item> items;
};

#endif // SELECTER_H
