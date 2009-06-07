#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QWidget>

class QGraphicsScene;
class QGraphicsView;
class QRuler;
class QUnit;

class PageView : public QWidget
{
    Q_OBJECT
public:
    PageView(QGraphicsScene * scene = 0, QWidget * parent = 0, Qt::WindowFlags f = 0);

    QGraphicsScene * scene() const;
    QGraphicsView * view() const;

protected:
    virtual void resizeEvent ( QResizeEvent * event );
//    virtual void scrollContentsBy ( int dx, int dy );
    virtual void mouseMoveEvent ( QMouseEvent * e );

private slots:
    void doHorizontalScroll(int value);
    void doVerticalScroll(int value);

private:
    QGraphicsScene * m_scene;
    QGraphicsView * m_view;
    QRuler * m_horizontalRuler;
    QRuler * m_verticalRuler;
    QUnit * m_unit;
};

#endif // PAGEVIEW_H
