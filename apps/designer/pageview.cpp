/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2009 by Alexander Mikhalov  (aka alFoX)                 *
 *              alexmi3@rambler.ru                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#include "pageview.h"
#include "QGridLayout"
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QDesktopWidget>
#include <QScrollBar>
#include <QMouseEvent>
#include <QTimer>
#include "qruler.h"
#include "qunit.h"
#include "selecter.h"

#define _scale 0.283465  // make scene ratio depend on rules

PageView::PageView(QGraphicsScene *  scene, QWidget * parent, Qt::WindowFlags f )
	: QWidget(parent, f ),
	m_scene(scene),
	m_zoom(1),
	m_range(QRect())
{
    m_view = new GraphicsView ( m_scene, this );
    m_view->centerOn( 0, 0 );
    m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);

    m_horizontalRuler = new QRuler(this, Qt::Horizontal);
    m_horizontalRuler->setShowMousePosition(true);
    m_horizontalRuler->setUnit(QUnit(QUnit::Millimeter));
    m_horizontalRuler->setRulerLength((qreal)m_scene->width() * m_zoom);

    m_verticalRuler = new QRuler(this, Qt::Vertical);
    m_verticalRuler->setShowMousePosition(true);
    m_verticalRuler->setUnit(QUnit(QUnit::Millimeter));
    m_verticalRuler->setRulerLength(  (qreal)m_scene->height()* m_zoom);

    setLayout(gridLayout);

    //	gridLayout->addWidget(m_horizontalRuler->tabChooser(), 0, 0);
    gridLayout->addWidget(m_horizontalRuler, 0, 1);
    gridLayout->addWidget(m_verticalRuler, 1, 0);
    gridLayout->addWidget(m_view , 1, 1);

    connect ( m_view->horizontalScrollBar() , SIGNAL (valueChanged(int)), this, SLOT(doHorizontalScroll(int) ) );
    connect ( m_view->verticalScrollBar() , SIGNAL (valueChanged(int)), this, SLOT(doVerticalScroll(int) ) );
    connect ( m_view, SIGNAL ( mousePositionChanged(QPoint) ), this, SLOT ( mousePositionChanged(QPoint) ) );

    m_selecter = new Selecter(scene);

    QTimer::singleShot(0, this, SLOT ( setZoomFitToPage())) ;
}

PageView::~PageView()
{
    delete m_selecter;
}
/*
void PageView::resizeEvent ( QResizeEvent * event )
{
    m_horizontalRuler->setRulerLength(m_scene->width() * m_zoom * _scale );
    m_verticalRuler->setRulerLength(  m_scene->height()* m_zoom * _scale );
}
*/

void PageView::mousePositionChanged(QPoint pos)
{
    m_horizontalRuler->updateMouseCoordinate(m_view->horizontalScrollBar()->value() + pos.x());
    m_verticalRuler->updateMouseCoordinate(m_view->verticalScrollBar()->value() + pos.y());
}

qreal PageView::zoom()
{
    return m_zoom;
}

void PageView::setZoom(qreal zoom)
{
    m_zoom = zoom;
    m_view->resetMatrix();
    m_view->scale(_scale * m_zoom, _scale * m_zoom);
    m_horizontalRuler->setScale(m_zoom);
    m_verticalRuler->setScale(m_zoom);
}

void PageView::setZoomFitToPage()
{
    setZoom( (qreal)(m_view->viewport()->width() - 4) / (qreal)m_scene->width() / _scale ); // 4 = gap
}

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

Selecter * PageView::selecter() const
{
    return m_selecter;
}

void PageView::setActiveRange(QRect rect)
{
    m_horizontalRuler->setActiveRange(rect.left(), rect.right() );
    m_verticalRuler->setActiveRange(rect.top(), rect.bottom() );
}

