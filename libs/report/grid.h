#ifndef GRID_H
#define GRID_H

#include <QtCore>

class Grid
{
public:
    Grid();

    static Grid * instance();

    void snap(bool x, bool y);
    void snap(bool s);
    qreal adjustX(qreal x) const;
    qreal adjustY(qreal y) const;
    QPointF adjustPoint(QPointF point) const;

    void setDeltaX(int x);
    void setDeltaY(int y);
    void setDelta(int d);

    int deltaX();
    int deltaY();

private:
    static Grid * m_instance;
    bool m_snapX;
    bool m_snapY;
    int m_deltaX;
    int m_deltaY;
};

#endif // GRID_H
