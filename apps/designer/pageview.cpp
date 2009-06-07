#include "pageview.h"
#include "QGridLayout"
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QScrollBar>
#include <QMouseEvent>
#include "qruler.h"
#include "qunit.h"

PageView::PageView(QGraphicsScene *  scene, QWidget * parent, Qt::WindowFlags f )
	: QWidget(parent, f ),
	m_scene(scene)
{
    m_view = new QGraphicsView ( m_scene, this );

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);

    m_horizontalRuler = new QRuler(this, Qt::Horizontal);
    m_horizontalRuler->setShowMousePosition(true);
    m_horizontalRuler->setUnit(QUnit());
//    m_horizontalRuler->setRulerLength(m_view->width());
//    m_horizontalRuler->setActiveRange(200,300/*gw->width()*/);

    m_verticalRuler = new QRuler(this, Qt::Vertical);
    m_verticalRuler->setShowMousePosition(true);
    m_verticalRuler->setUnit(QUnit());
//    m_verticalRuler->setRulerLength( m_view->height() );
//    m_verticalRuler->setActiveRange(0, m_view->height() );

    setLayout(gridLayout);

    //	gridLayout->addWidget(m_horizontalRuler->tabChooser(), 0, 0);
    gridLayout->addWidget(m_horizontalRuler, 0, 1);
    gridLayout->addWidget(m_verticalRuler, 1, 0);
    gridLayout->addWidget(m_view , 1, 1);

    //connect ( m_view->horizontalScrollBar() , SIGNAL (valueChanged(int)), m_horizontalRuler, SLOT(setOffset(int) ) );
    connect ( m_view->horizontalScrollBar() , SIGNAL (valueChanged(int)), this, SLOT(doHorizontalScroll(int) ) );
    connect ( m_view->verticalScrollBar() , SIGNAL (valueChanged(int)), this, SLOT(doVerticalScroll(int) ) );

    this->setMouseTracking(true);
}

void PageView::resizeEvent ( QResizeEvent * event )
{
    QPoint p = m_view->mapFromScene(m_scene->width(), m_scene->height());
    m_horizontalRuler->setRulerLength(p.x() );
    m_verticalRuler->setRulerLength( p.y() );
}

void PageView::mouseMoveEvent ( QMouseEvent * e )
{
    qDebug("mouse ");
    m_horizontalRuler->updateMouseCoordinate(e->x());
    m_verticalRuler->updateMouseCoordinate(e->y());
}

/*
void PageView::scrollContentsBy ( int dx, int dy )
{
    m_horizontalRuler->setOffset(dx);
    m_verticalRuler->setOffset(dy);
}
*/

void PageView::doHorizontalScroll(int value)
{
      m_horizontalRuler->setOffset(- value );
}

void PageView::doVerticalScroll(int value)
{
      m_verticalRuler->setOffset(- value);
}

QGraphicsScene * PageView::scene() const
{
    return m_scene;
}

QGraphicsView * PageView::view() const
{
    return m_view;
}
