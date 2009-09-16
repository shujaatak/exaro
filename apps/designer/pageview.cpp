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
#include "mainwindow.h"
#include "message.h"

#include <QGLWidget>

#define _scale 0.283465  // make scene ratio depend on rules

PageView::PageView(QGraphicsScene * scene, mainWindow * mw, QWidget * parent, Qt::WindowFlags f )
	: QWidget(parent, f ),
	m_scene(scene),
	m_zoom(1),
	m_range(QRect()),
	m_mw(mw)
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

    gridLayout->addWidget(m_horizontalRuler->tabChooser(), 0, 0);
    gridLayout->addWidget(m_horizontalRuler, 0, 1);
    gridLayout->addWidget(m_verticalRuler, 1, 0);
    gridLayout->addWidget(m_view , 1, 1);

    connect ( m_view->horizontalScrollBar() , SIGNAL (valueChanged(int)), this, SLOT(doHorizontalScroll(int) ) );
    connect ( m_view->verticalScrollBar() , SIGNAL (valueChanged(int)), this, SLOT(doVerticalScroll(int) ) );
    connect ( m_view, SIGNAL ( mousePositionChanged(QPoint) ), this, SLOT ( mousePositionChanged(QPoint) ) );
    connect ( m_view, SIGNAL(addItem(Report::ItemInterface*,QPointF)), this, SIGNAL(addItem(Report::ItemInterface*,QPointF)) );
    connect ( m_view, SIGNAL(doubleClick(Report::ItemInterface*)), this, SIGNAL(doubleClick(Report::ItemInterface*)) );

    m_selecter = new Selecter(scene, m_mw);
    connect ( m_selecter, SIGNAL(itemMoved(Report::ItemInterface, QPointF)), this, SIGNAL(selectionMoved(Report::ItemInterface, QPointF)) );
    connect ( m_scene , SIGNAL(destroyed()), this, SLOT (sceneDestroyed()));

//    m_horizontalRuler->setShowTabs(true);
//    m_horizontalRuler->setFirstLineIndent(200);
//    m_horizontalRuler->setEndIndent(300);
//    m_horizontalRuler->setParagraphIndent(400);
//    m_horizontalRuler->setHotSpot(250, 0);
//    m_horizontalRuler->setShowSelectionBorders(true);
//    m_horizontalRuler->updateSelectionBorders(100, 300);

    QTimer::singleShot(0, this, SLOT ( setZoomFitToPage())) ;
}

PageView::~PageView()
{
//    if (m_selecter)
//    {
//	m_selecter->free();
//	delete m_selecter;
//    }
}

void PageView::sceneDestroyed()
{
    delete m_selecter;
    m_selecter = 0;
}

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

bool PageView::hasSelection()
{
    if (!m_selecter)
	return false;
    return m_selecter->hasSelection()?true:false;
}

QList<Report::ItemInterface *> PageView::selectedItems()
{
    if (!m_selecter)
	return QList<Report::ItemInterface *>();
    return m_selecter->selectedItems();
}

QObject * PageView::activeObject()
{
    if (!m_selecter)
	return 0;
    return m_selecter->activeObject();
}

QPointF PageView::activeObjectLastPressPos()
{
    if (!m_selecter)
	return QPointF();
    return m_selecter->activeObjectLastPressPos();
}

void PageView::beforeOuterChanging()
{
    if (m_selecter)
	m_selecter->store();
}

void PageView::afterOuterChanging()
{
    if (m_selecter)
	m_selecter->restore();
}





///-----------GraphicsView

GraphicsView::GraphicsView ( QGraphicsScene * scene, QWidget * parent )
	: QGraphicsView(scene, parent)
	, m_dragSourceItem(0)
{
    this->setAcceptDrops( true );
//    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
//    setViewport(new QGLWidget(this));
}

void GraphicsView::mouseMoveEvent ( QMouseEvent * e )
{
    QGraphicsView::mouseMoveEvent( e );
    emit mousePositionChanged ( e->pos () );
}

void GraphicsView::mouseDoubleClickEvent ( QMouseEvent * event )
{
    foreach (QGraphicsItem* item, items(event->pos()))
	if (dynamic_cast<Report::ItemInterface *>(item) )
	{
	    emit doubleClick(static_cast<Report::ItemInterface *>(item));
	    return;
	}
}

void GraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
    m_canDrop = false;
    m_lastCheckedItem = 0;

    if (dynamic_cast<PageView*>(this->parent()))
	m_dragSourceItem = dynamic_cast<Report::ItemInterface*>((dynamic_cast<PageView*>(this->parent()))->m_mw->reportEngine()->findItemByClassName( qPrintable(event->mimeData()->text().section("::",1,1) )) );
}

void GraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    event->ignore();
    m_canDrop = false;
    if((!scene()) || (!m_dragSourceItem))
	return;

    // speedup
    if (m_lastCheckedItem == this->itemAt( event->pos() ) )
	return;

    bool itemExist = false;
    foreach (QGraphicsItem* item, items(event->pos()))
	if (dynamic_cast<Report::ItemInterface *>(item) )
	{
	    itemExist = true;
	    if (dynamic_cast<Report::ItemInterface *>(item)->canContain(m_dragSourceItem) )
	    {
		event->acceptProposedAction();
		m_canDrop = true;
		return;
	    }
	}

    if ((!itemExist) && dynamic_cast<Report::BandInterface *>(m_dragSourceItem))
    {
	event->acceptProposedAction();
	m_canDrop = true;
    }
}

void GraphicsView::dragLeaveEvent ( QDragLeaveEvent * event )
{

}

void GraphicsView::dropEvent(QDropEvent *event)
{
    if (m_canDrop)
	emit addItem(m_dragSourceItem, this->mapToScene(event->pos() ) );
}
