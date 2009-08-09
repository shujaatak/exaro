#ifndef SELECTER_H
#define SELECTER_H
#include <QList>
#include "iteminterface.h"
#include "itemselection.h"
#include "reportengine.h"

class GraphicsItemGroup;
class QGraphicsScene;
class mainWindow;

struct Item
{
    Report::ItemInterface * item;
    QPointF pos;
//    Report::ItemInterface * parent;
    ItemSelection * sel;
};

class Selecter: public QObject
{
    Q_OBJECT
public:
    Selecter( QGraphicsScene * scene, mainWindow * mw);
    ~Selecter();

    QObject * itemSelected(Report::ItemInterface * item, QPointF pos, Qt::KeyboardModifiers keys);
    QObject * itemSelected(QObject * object, QPointF pos, Qt::KeyboardModifiers keys);

    QObject * activeObject();
    QPointF activeObjectLastPressPos();

    void store();
    void restore();

    bool haveSelection();
    QList<Report::ItemInterface *> selectedItems();
    
private:
    void append (Report::ItemInterface * item);
    void remove (Report::ItemInterface * item);
    void free();


public slots:
    void itemMoved(Report::ItemInterface * item, QPointF oldPos);


private:
    QObject * _itemSelected(Report::ItemInterface * item, QPointF pos, Qt::KeyboardModifiers keys);
    void setGuideItem(Report::ItemInterface * item);

private slots:
    void itemGeometryChanged(QObject* item, QRectF);
//    void sceneDestroyed();

private:
    QList<Item> items;
    QList<Item> storedItems;
    QObject * m_activeObject;
    QPointF m_activeObjectPressPos;
    mainWindow* m_mw;
};

class GraphicsItemGroup: public QGraphicsItemGroup
{
    protected:
    void paint ( QPainter * /*painter*/, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/) {};
};


#endif // SELECTER_H
