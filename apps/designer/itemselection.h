#ifndef ITEMSELECTION_H
#define ITEMSELECTION_H

//#include "formeditor_global.h"
//#include <invisible_widget_p.h>
#include "iteminterface.h"

#include <QtCore/QHash>
#include <QtCore/QPointer>

//class QDesignerFormEditorInterface;
class QMouseEvent;
class QPaintEvent;

//class FormWindow;
class ItemSelection;

class ItemHandle: public QGraphicsItem
{
//    Q_OBJECT
public:
    enum Type
    {
        LeftTop,
        Top,
        RightTop,
        Right,
        RightBottom,
        Bottom,
        LeftBottom,
        Left,

        TypeCount
    };

    ItemHandle(Type t, ItemSelection *s);
    void setItem(Report::ItemInterface *i);
    void setActive(bool a);
    void updateCursor();

    void setEnabled(bool) {}

    QRectF boundingRect () const;
//    QDesignerFormEditorInterface *core() const;

protected:
//    void paintEvent(QPaintEvent *e);
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
//    void changeGridLayoutItemSpan();
//    void changeFormLayoutItemSpan();
    void trySetGeometry(Report::ItemInterface *i, int x, int y, int width, int height);
    void tryResize(Report::ItemInterface *i, int width, int height);

private:
    Report::ItemInterface *m_item;
    const Type m_type;
    QPointF m_origPressPos;
//    FormWindow *m_formWindow;
    ItemSelection *m_sel;
    QRectF m_geom, m_origGeom;
    bool m_active;
};

class ItemSelection: public QGraphicsItem
{
//    Q_OBJECT
public:
//    ItemSelection(FormWindow *parent);
    ItemSelection();
//    ItemSelection(Report::ItemInterface *parent);

    void setItem(Report::ItemInterface *i);
    bool isUsed() const;

    void updateActive();
    void updateGeometry();
    void hide();
    void show();
    void update();

    Report::ItemInterface *item() const;
    void setGuideItem (Report::ItemInterface *i);

//    QDesignerFormEditorInterface *core() const;


//    enum  ItemState { UnlaidOut, LaidOut, ManagedGridLayout, ManagedFormLayout };
//    static ItemState itemState(Report::ItemInterface *i);

    QRectF boundingRect () const;
protected:
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    bool sceneEventFilter(QGraphicsItem * watched, QEvent * event );
private:
    ItemHandle *m_handles[ItemHandle::TypeCount];
    QPointer<Report::ItemInterface> m_item;
    QRectF m_boundingRect;
    QPointer<Report::ItemInterface> m_guideItem;
//    FormWindow *m_formWindow;
};


#endif // ITEMSELECTION_H
