#ifndef SELECTER_H
#define SELECTER_H
#include <QList>
#include "iteminterface.h"
#include "itemselection.h"

class GraphicsItemGroup;
class QGraphicsScene;

struct Item
{
    Report::ItemInterface * item;
    qreal zValue;
    QPointF pos;
    Report::ItemInterface * parent;
    ItemSelection * sel;
};

class Selecter: public QObject
{
    Q_OBJECT
public:
    Selecter( QGraphicsScene * scene);
    ~Selecter();

    QObject * itemSelected(Report::ItemInterface * item, QPointF pos, Qt::KeyboardModifiers keys);
    QObject * itemSelected(QObject * object, QPointF pos, Qt::KeyboardModifiers keys);

    QObject * activeObject();
    QPointF activeObjectLastPressPos();

    void add (Report::ItemInterface * item);
    void remove (Report::ItemInterface * item);
    void free();

    void store();
    void restore();

public slots:
    void itemMoved(Report::ItemInterface * item, QPointF oldPos);

private:
    QObject * _itemSelected(Report::ItemInterface * item, QPointF pos, Qt::KeyboardModifiers keys);
    void setGuideItem(Report::ItemInterface * item);
//private slots:
//    void sceneDestroyed();

private:
    GraphicsItemGroup * sItem;
    QList<Item> items;
    QList<Item> storedItems;
    QObject * m_activeObject;
    QPointF m_activeObjectPressPos;
};

class GraphicsItemGroup: public QGraphicsItemGroup
{
    protected:
    void paint ( QPainter * /*painter*/, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/) {};
};


#endif // SELECTER_H
