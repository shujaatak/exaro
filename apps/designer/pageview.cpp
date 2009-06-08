/***************************************************************************
 *   Copyright (C) 2009 by Mikhalov Alexander                              *
 *   alexmi3@rambler.ru                                                    *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "pageview.h"
#include "QGridLayout"
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QScrollBar>
#include <QMouseEvent>
#include "qruler.h"
#include "qunit.h"

/*
GraphicsView::GraphicsView ( QGraphicsScene * scene, QWidget * parent)
	:QGraphicsView (scene, parent)
{

}
*/

PageView::PageView(QGraphicsScene *  scene, QWidget * parent, Qt::WindowFlags f )
	: QWidget(parent, f ),
	m_scene(scene)
{
    m_view = new GraphicsView ( m_scene, this );
//    m_unit = new QUnit();
//    m_unit->

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);

    m_horizontalRuler = new QRuler(this, Qt::Horizontal);
    m_horizontalRuler->setShowMousePosition(true);
    m_horizontalRuler->setUnit(QUnit(QUnit::Millimeter));
//    m_horizontalRuler->setRulerLength(m_view->width());
//    m_horizontalRuler->setActiveRange(200,300/*gw->width()*/);

    m_verticalRuler = new QRuler(this, Qt::Vertical);
    m_verticalRuler->setShowMousePosition(true);
    m_verticalRuler->setUnit(QUnit(QUnit::Millimeter));
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
    connect ( m_view, SIGNAL ( mousePositionChanged(QPoint) ), this, SLOT ( mousePositionChanged(QPoint) ) );

}

void PageView::resizeEvent ( QResizeEvent * event )
{
    QPoint p = m_view->mapFromScene(m_scene->width(), m_scene->height());
    m_horizontalRuler->setRulerLength(p.x() );
    m_verticalRuler->setRulerLength( p.y() );
}

void PageView::mousePositionChanged(QPoint pos)
{
    m_horizontalRuler->updateMouseCoordinate(m_view->horizontalScrollBar()->value() + pos.x());
    m_verticalRuler->updateMouseCoordinate(m_view->verticalScrollBar()->value() + pos.y());
}

/*
void PageView::mouseMoveEvent ( QMouseEvent * e )
{
    qDebug("mouse ");
    m_horizontalRuler->updateMouseCoordinate(e->x());
    m_verticalRuler->updateMouseCoordinate(e->y());
}
*/

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
    return (QGraphicsView *)m_view;
}
