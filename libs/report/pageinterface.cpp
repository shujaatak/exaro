/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
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

#include <QPrinter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QCursor>
#include <QKeyEvent>
#include <QGraphicsView>

#include "pageinterface.h"
#include "bandinterface.h"


namespace Report
{

PageInterface::PageInterface(QObject *parent)
		: QGraphicsScene(parent), m_contextMenu(0), m_magnets(0), m_magnetArea(1),m_paperBorder(0),m_pageBorder(0),m_showGrid(false)
{
	m_pageSize = A4;
	m_orientation = Portrait;
	setPageSize(A4);
	setOrientation(Portrait);
	QFont m_font=QFont("Serif");
	m_font.setPointSizeF(3.5);
	m_font.setStyleStrategy(QFont::PreferMatch);
	m_font.setStyleStrategy(QFont::ForceOutline);
	setFont(m_font);
}

QRectF PageInterface::geometry()
{
	return m_geometry;
}

void PageInterface::setGeometry(QRectF rect)
{
	m_geometry = rect & sceneRect();
	drawBorder();
	emit geometryChanged(m_geometry);
}


PageInterface::PageSize PageInterface::pageSize()
{
	return m_pageSize;
}

void PageInterface::setPageSize(PageInterface::PageSize pageSize)
{
	m_pageSize = pageSize;
	updatePageRect();
	m_paperRect = sceneRect();
	setGeometry(QRect(15/UNIT, 10/UNIT, sceneRect().width() - 15/UNIT*2, sceneRect().height()-10/UNIT*2));
}

QRectF PageInterface::paperRect()
{
	return m_paperRect;
}

void PageInterface::setPaperRect(QRectF paperRect)
{
	m_paperRect = paperRect;
}


PageInterface::Orientation PageInterface::orientation()
{
	return m_orientation;
}

void PageInterface::setOrientation(PageInterface::Orientation orientation)
{
	m_orientation = orientation;
	updatePageRect();
	m_paperRect = sceneRect();
	setGeometry(QRect(15/UNIT, 10/UNIT, sceneRect().width() - 15/UNIT*2, sceneRect().height()-10/UNIT*2));
}

int PageInterface::magnetArea()
{
	return m_magnetArea;
}

void PageInterface::setMagnetArea(int magnetArea)
{
	m_magnetArea = magnetArea;
}

void PageInterface::updatePageRect()
{
	QPrinter p;
	p.setOutputFormat(QPrinter::PdfFormat);
	p.setOrientation((QPrinter::Orientation)orientation());
	p.setPageSize((QPrinter::PageSize)pageSize());
	setSceneRect(0, 0, p.paperRect(QPrinter::Millimeter).width()/UNIT, p.paperRect(QPrinter::Millimeter).height()/UNIT);
}

void PageInterface::drawMagnets(ItemInterface* item)
{
	foreach(QGraphicsItem * it, m_gideLines)
		removeItem(it);

	m_gideLines.clear();

	if (!m_magnets)
		return;

	item->setToolTip(tr("Press Ctrl key to ignore magnets when dragging the object"));

	QPen p;
	p.setWidth(3);
	p.setColor(Qt::red);
	p.setStyle(Qt::DotLine);

	foreach(QGraphicsItem * it, items())
	{
		ItemInterface* ite = dynamic_cast<ItemInterface*>(it);

		if (!ite || !item->parentItem() || ite == item)
			continue;

		if ((m_magnets&Left) && abs(item->mapToScene(QPointF(0, 0)).x() - ite->mapToScene(QPointF(0, 0)).x()) <= m_magnetArea)
		{
			item->setGeometry(QRectF(ite->mapToItem(item->parentItem(), QPointF(0, 0)).x(), item->geometry().y(), item->geometry().width(), item->geometry().height()));

			if (item->mapToScene(0, 0).y() < ite->mapToScene(0, ite->geometry().height()).y())
				m_gideLines.push_back(addLine(item->mapToScene(0, 0).x(), item->mapToScene(0, 0).y(), item->mapToScene(0, 0).x(), ite->mapToScene(0, ite->geometry().height()).y(), p));
			else
				m_gideLines.push_back(addLine(item->mapToScene(0, 0).x(), ite->mapToScene(0, 0).y(), item->mapToScene(0, 0).x(), item->mapToScene(0, item->geometry().height()).y(), p));
		}

		if ((m_magnets&Left) && abs(item->mapToScene(QPointF(0, 0)).x() - ite->mapToScene(QPointF(ite->geometry().width(), 0)).x()) <= m_magnetArea)
		{
			item->setGeometry(QRectF(ite->mapToItem(item->parentItem(), QPointF(ite->geometry().width(), 0)).x(), item->geometry().y(), item->geometry().width(), item->geometry().height()));

			if (item->mapToScene(0, 0).y() < ite->mapToScene(0, ite->geometry().height()).y())
				m_gideLines.push_back(addLine(item->mapToScene(0, 0).x(), item->mapToScene(0, 0).y(), item->mapToScene(0, 0).x(), ite->mapToScene(0, ite->geometry().height()).y(), p));
			else
				m_gideLines.push_back(addLine(item->mapToScene(0, 0).x(), ite->mapToScene(0, 0).y(), item->mapToScene(0, 0).x(), item->mapToScene(0, item->geometry().height()).y(), p));
		}

		if ((m_magnets&Right) && abs(item->mapToScene(item->geometry().width(), 0).x() - ite->mapToScene(0, 0).x()) <= m_magnetArea)
		{
			item->setGeometry(QRectF(ite->mapToItem(item->parentItem(), 0, 0).x() - item->geometry().width(), item->geometry().y(), item->geometry().width(), item->geometry().height()));

			if (item->mapToScene(item->geometry().width(), 0).y() < ite->mapToScene(ite->geometry().width(), ite->geometry().height()).y())
				m_gideLines.push_back(addLine(item->mapToScene(item->geometry().width(), 0).x(), item->mapToScene(item->geometry().width(), 0).y(), item->mapToScene(item->geometry().width(), 0).x(), ite->mapToScene(ite->geometry().width(), ite->geometry().height()).y(), p));
			else
				m_gideLines.push_back(addLine(item->mapToScene(item->geometry().width(), 0).x(), ite->mapToScene(ite->geometry().width(), 0).y(), item->mapToScene(item->geometry().width(), 0).x(), item->mapToScene(item->geometry().width(), item->geometry().height()).y(), p));
		}

		if ((m_magnets&Right) && abs(item->mapToScene(item->geometry().width(), 0).x() - ite->mapToScene(ite->geometry().width(), 0).x()) <= m_magnetArea)
		{
			item->setGeometry(QRectF(ite->mapToItem(item->parentItem(), ite->geometry().width(), 0).x() - item->geometry().width(), item->geometry().y(), item->geometry().width(), item->geometry().height()));

			if (item->mapToScene(item->geometry().width(), 0).y() < ite->mapToScene(ite->geometry().width(), ite->geometry().height()).y())
				m_gideLines.push_back(addLine(item->mapToScene(item->geometry().width(), 0).x(), item->mapToScene(item->geometry().width(), 0).y(), ite->mapToScene(ite->geometry().width(), 0).x(), ite->mapToScene(ite->geometry().width(), ite->geometry().height()).y(), p));
			else
				m_gideLines.push_back(addLine(ite->mapToScene(ite->geometry().width(), 0).x(), ite->mapToScene(ite->geometry().width(), 0).y(), item->mapToScene(item->geometry().width(), 0).x(), item->mapToScene(item->geometry().width(), item->geometry().height()).y(), p));
		}

		if ((m_magnets&Top) && abs(item->mapToScene(QPointF(0, 0)).y() - ite->mapToScene(QPointF(0, 0)).y()) <= m_magnetArea)
		{
			item->setGeometry(QRectF(item->geometry().x(), ite->mapToItem(item->parentItem(), QPointF(0, 0)).y(), item->geometry().width(), item->geometry().height()));
			m_gideLines.push_back(addLine(item->mapToScene(0, 0).x(), item->mapToScene(0, 0).y(), ite->mapToScene(0, 0).x(), item->mapToScene(0, 0).y(), p));
		}

		if ((m_magnets&Top) && abs(item->mapToScene(QPointF(0, 0)).y() - ite->mapToScene(QPointF(0, ite->geometry().height())).y()) <= m_magnetArea)
		{
			item->setGeometry(QRectF(item->geometry().x(), ite->mapToItem(item->parentItem(), QPointF(0, ite->geometry().height())).y(), item->geometry().width(), item->geometry().height()));
			m_gideLines.push_back(addLine(item->mapToScene(0, 0).x(), item->mapToScene(0, 0).y(), ite->mapToScene(0, 0).x(), item->mapToScene(0, 0).y(), p));
		}

		if ((m_magnets&Bottom) && abs(item->mapToScene(QPointF(0, item->geometry().height())).y() - ite->mapToScene(QPointF(0, ite->geometry().height())).y()) <= m_magnetArea)
		{
			item->setGeometry(QRectF(item->geometry().x(), ite->mapToItem(item->parentItem(), QPointF(0, ite->geometry().height())).y() - item->geometry().height(), item->geometry().width(), item->geometry().height()));
			m_gideLines.push_back(addLine(item->mapToScene(0, 0).x(), item->mapToScene(0, item->geometry().height()).y(), ite->mapToScene(0, 0).x(), item->mapToScene(0, item->geometry().height()).y(), p));
		}

		if ((m_magnets&Bottom) && abs(item->mapToScene(QPointF(0, item->geometry().height())).y() - ite->mapToScene(QPointF(0, 0)).y()) <= m_magnetArea)
		{
			item->setGeometry(QRectF(item->geometry().x(), ite->mapToItem(item->parentItem(), QPointF(0, 0)).y() - item->geometry().height(), item->geometry().width(), item->geometry().height()));
			m_gideLines.push_back(addLine(item->mapToScene(0, 0).x(), item->mapToScene(0, item->geometry().height()).y(), ite->mapToScene(0, 0).x(), item->mapToScene(0, item->geometry().height()).y(), p));
		}

		if ((m_magnets&Horizontal) && abs(item->mapToScene(QPointF(0, item->geometry().height() / 2)).y() - ite->mapToScene(QPointF(0, ite->geometry().height() / 2)).y()) <= m_magnetArea)
		{
			item->setGeometry(QRectF(item->geometry().x(), ite->mapToItem(item->parentItem(), QPointF(0, ite->geometry().height() / 2)).y() - item->geometry().height() / 2, item->geometry().width(), item->geometry().height()));
			m_gideLines.push_back(addLine(item->mapToScene(0, 0).x(), item->mapToScene(0, item->geometry().height() / 2).y(), ite->mapToScene(0, 0).x(), item->mapToScene(0, item->geometry().height() / 2).y(), p));
		}

		if ((m_magnets&Vertical) && abs(item->mapToScene(QPointF(item->geometry().width() / 2, 0)).x() - ite->mapToScene(QPointF(ite->geometry().width() / 2, 0)).x()) <= m_magnetArea)
		{
			item->setGeometry(QRectF(ite->mapToItem(item->parentItem(), QPointF(ite->geometry().width() / 2, 0)).x() - item->geometry().width() / 2, item->geometry().y(), item->geometry().width(), item->geometry().height()));
			m_gideLines.push_back(addLine(item->mapToScene(item->geometry().width() / 2, 0).x(), item->mapToScene(0, 0).y(), item->mapToScene(item->geometry().width() / 2, 0).x(), ite->mapToScene(0, ite->geometry().height()).y(), p));
		}

	}
}

void PageInterface::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	Report::ItemInterface* item = 0;

	if (itemAt(mouseEvent->scenePos()))
		item = dynamic_cast<Report::ItemInterface*>(itemAt(mouseEvent->scenePos()));


	if (mouseEvent->buttons() == Qt::NoButton && item)
	{

		int pc = item->posibleResizeCurrsor(item->mapFromScene(mouseEvent->scenePos()));
		if (ItemInterface::Fixed != pc)
		{
			if ((pc&ItemInterface::ResizeTop && pc&ItemInterface::ResizeLeft) || (pc&ItemInterface::ResizeBottom && pc&ItemInterface::ResizeRight))
				item->setCursor(QCursor(Qt::SizeFDiagCursor));
			else
				if ((pc&ItemInterface::ResizeTop && pc&ItemInterface::ResizeRight) || (pc&ItemInterface::ResizeBottom && pc&ItemInterface::ResizeLeft))
					item->setCursor(QCursor(Qt::SizeBDiagCursor));
				else
					if ((pc&ItemInterface::ResizeTop) || (pc&ItemInterface::ResizeBottom))
						item->setCursor(QCursor(Qt::SizeVerCursor));
					else
						if ((pc&ItemInterface::ResizeRight) || (pc&ItemInterface::ResizeLeft))
							item->setCursor(QCursor(Qt::SizeHorCursor));
						else
							if (pc&ItemInterface::FixedPos)
								item->setCursor(QCursor(Qt::ArrowCursor));
		}
		else
			if (ItemInterface::Fixed == pc)
				item->setCursor(QCursor(Qt::OpenHandCursor));
	}

	QGraphicsScene::mouseMoveEvent(mouseEvent);

	if (mouseEvent->buttons() != Qt::LeftButton)
		return;

	item = 0;

	if (selectedItems().size())
		item = dynamic_cast<Report::ItemInterface*>(selectedItems()[0]);

	if (item && !(mouseEvent->modifiers()& Qt::ControlModifier))
		drawMagnets(item);
}

void PageInterface::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF mousePos(event->buttonDownScenePos(Qt::LeftButton).x(),
			event->buttonDownScenePos(Qt::LeftButton).y());
	movingItem = itemAt(mousePos.x(), mousePos.y());

	if (movingItem != 0 && event->button() == Qt::LeftButton) 
		mouseOldPos = movingItem->pos();
  
	QGraphicsScene::mousePressEvent(event);
	if (event->buttons() == Qt::LeftButton)
	{
		if  (!itemAt(event->scenePos()))
			emit itemSelected(parent(), event->scenePos());
		else
			if (itemAt(event->scenePos()) == m_paperBorder || itemAt(event->scenePos()) == m_pageBorder)
				emit itemSelected(this, event->scenePos());
	}
}

void PageInterface::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	foreach(QGraphicsItem * item, m_gideLines)
		removeItem(item);

	m_gideLines.clear();

	if (movingItem != 0 && event->button() == Qt::LeftButton) 
	{
		if (mouseOldPos != movingItem->pos())
			emit itemMoved(dynamic_cast<QObject *>(movingItem), mouseOldPos);
		movingItem = 0;
	}
	QGraphicsScene::mouseReleaseEvent(event);
}

void PageInterface::keyReleaseEvent(QKeyEvent * keyEvent)
{
	foreach(QGraphicsItem * item, m_gideLines)
		removeItem(item);
	m_gideLines.clear();
	QGraphicsScene::keyReleaseEvent(keyEvent);
}

void PageInterface::keyPressEvent(QKeyEvent * keyEvent)
{
	if (keyEvent->modifiers()==Qt::NoModifier || (keyEvent->key()!=Qt::Key_Left && keyEvent->key()!=Qt::Key_Right && keyEvent->key()!=Qt::Key_Up && keyEvent->key()!=Qt::Key_Down))
	{
		QGraphicsScene::keyPressEvent(keyEvent);
		return;
	}

	if (keyEvent->modifiers() & Qt::AltModifier)
		foreach(QGraphicsItem * item, selectedItems())
		{
			if (dynamic_cast<Report::BandInterface*>(item))
				continue;

			if (dynamic_cast<Report::ItemInterface*>(item))
			{
				QRectF rect;
				switch(keyEvent->key())
				{
					case Qt::Key_Left:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x()-5,dynamic_cast<Report::ItemInterface*>(item)->geometry().y(),dynamic_cast<Report::ItemInterface*>(item)->geometry().width(),dynamic_cast<Report::ItemInterface*>(item)->geometry().height());
						break;
					case Qt::Key_Right:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x()+5,dynamic_cast<Report::ItemInterface*>(item)->geometry().y(),dynamic_cast<Report::ItemInterface*>(item)->geometry().width(),dynamic_cast<Report::ItemInterface*>(item)->geometry().height());
						break;
					case Qt::Key_Up:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x(),dynamic_cast<Report::ItemInterface*>(item)->geometry().y()-5,dynamic_cast<Report::ItemInterface*>(item)->geometry().width(),dynamic_cast<Report::ItemInterface*>(item)->geometry().height());
						break;
					case Qt::Key_Down:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x(),dynamic_cast<Report::ItemInterface*>(item)->geometry().y()+5,dynamic_cast<Report::ItemInterface*>(item)->geometry().width(),dynamic_cast<Report::ItemInterface*>(item)->geometry().height());
						break;
				}
				dynamic_cast<Report::ItemInterface*>(item)->setGeometry(rect);
				drawMagnets(dynamic_cast<Report::ItemInterface*>(item));
			}
		}

	if (keyEvent->modifiers() & Qt::ControlModifier)
		foreach(QGraphicsItem * item, selectedItems())
		{
			if (dynamic_cast<Report::BandInterface*>(item))
				continue;

			if (dynamic_cast<Report::ItemInterface*>(item))
			{
				QRectF rect;
				switch(keyEvent->key())
				{
					case Qt::Key_Left:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x()-1,dynamic_cast<Report::ItemInterface*>(item)->geometry().y(),dynamic_cast<Report::ItemInterface*>(item)->geometry().width(),dynamic_cast<Report::ItemInterface*>(item)->geometry().height());
						break;
					case Qt::Key_Right:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x()+1,dynamic_cast<Report::ItemInterface*>(item)->geometry().y(),dynamic_cast<Report::ItemInterface*>(item)->geometry().width(),dynamic_cast<Report::ItemInterface*>(item)->geometry().height());
						break;
					case Qt::Key_Up:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x(),dynamic_cast<Report::ItemInterface*>(item)->geometry().y()-1,dynamic_cast<Report::ItemInterface*>(item)->geometry().width(),dynamic_cast<Report::ItemInterface*>(item)->geometry().height());
						break;
					case Qt::Key_Down:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x(),dynamic_cast<Report::ItemInterface*>(item)->geometry().y()+1,dynamic_cast<Report::ItemInterface*>(item)->geometry().width(),dynamic_cast<Report::ItemInterface*>(item)->geometry().height());
						break;
				}
				dynamic_cast<Report::ItemInterface*>(item)->setGeometry(rect);
				drawMagnets(dynamic_cast<Report::ItemInterface*>(item));
			}
		}

	if (keyEvent->modifiers() & Qt::ShiftModifier)
		foreach(QGraphicsItem * item, selectedItems())
		{
			if (dynamic_cast<Report::ItemInterface*>(item))
			{
				QRectF rect;
				switch(keyEvent->key())
				{
					case Qt::Key_Left:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x(),dynamic_cast<Report::ItemInterface*>(item)->geometry().y(),dynamic_cast<Report::ItemInterface*>(item)->geometry().width()-1,dynamic_cast<Report::ItemInterface*>(item)->geometry().height());
						break;
					case Qt::Key_Right:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x(),dynamic_cast<Report::ItemInterface*>(item)->geometry().y(),dynamic_cast<Report::ItemInterface*>(item)->geometry().width()+1,dynamic_cast<Report::ItemInterface*>(item)->geometry().height());
						break;
					case Qt::Key_Up:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x(),dynamic_cast<Report::ItemInterface*>(item)->geometry().y(),dynamic_cast<Report::ItemInterface*>(item)->geometry().width(),dynamic_cast<Report::ItemInterface*>(item)->geometry().height()-1);
						break;
					case Qt::Key_Down:
						rect=QRectF(dynamic_cast<Report::ItemInterface*>(item)->geometry().x(),dynamic_cast<Report::ItemInterface*>(item)->geometry().y(),dynamic_cast<Report::ItemInterface*>(item)->geometry().width(),dynamic_cast<Report::ItemInterface*>(item)->geometry().height()+1);
						break;
				}
				dynamic_cast<Report::ItemInterface*>(item)->setGeometry(rect);
				drawMagnets(dynamic_cast<Report::ItemInterface*>(item));
			}
		}
}


void PageInterface::setContextMenu(QMenu * menu)
{
	m_contextMenu = menu;
}

void PageInterface::contextMenuEvent(QGraphicsSceneContextMenuEvent * contextMenuEvent)
{
	if (m_contextMenu)
		m_contextMenu->exec(contextMenuEvent->screenPos());
}

void PageInterface::setLeftMagnet(bool magneted)
{
	if (magneted)
		m_magnets |= Left;
	else
	{
		int a = 0xffff;
		a ^= Left;
		m_magnets &= a;
	}
}

void PageInterface::setRightMagnet(bool magneted)
{
	if (magneted)
		m_magnets |= Right;
	else
	{
		int a = 0xffff;
		a ^= Right;
		m_magnets &= a;
	}
}

void PageInterface::setTopMagnet(bool magneted)
{
	if (magneted)
		m_magnets |= Top;
	else
	{
		int a = 0xffff;
		a ^= Top;
		m_magnets &= a;
	}
}

void PageInterface::setBottomMagnet(bool magneted)
{
	if (magneted)
		m_magnets |= Bottom;
	else
	{
		int a = 0xffff;
		a ^= Bottom;
		m_magnets &= a;
	}
}

void PageInterface::setHorizontalMagnet(bool magneted)
{
	if (magneted)
		m_magnets |= Horizontal;
	else
	{
		int a = 0xffff;
		a ^= Horizontal;
		m_magnets &= a;
	}
}

void PageInterface::setVerticalMagnet(bool magneted)
{
	if (magneted)
		m_magnets |= Vertical;
	else
	{
		int a = 0xffff;
		a ^= Vertical;
		m_magnets &= a;
	}
}

void PageInterface::bandDestroyed(int type, int order)
{
	foreach(QObject *item, children())
		if (dynamic_cast<Report::BandInterface*>(item))
		{
			if (dynamic_cast<BandInterface*>(item)->bandType() == type && dynamic_cast<BandInterface*>(item)->order() > order)
				dynamic_cast<BandInterface*>(item)->setOrder(dynamic_cast<BandInterface*>(item)->order() - 1);
			if (dynamic_cast<BandInterface*>(item)->bandType() > type)
				dynamic_cast<BandInterface*>(item)->setOrder(dynamic_cast<BandInterface*>(item)->order());
		}
}

bool PageInterface::showGrid()
{
	return m_showGrid;
}

void PageInterface::setShowGrid(bool showGrid)
{
	m_showGrid=showGrid;
	drawBorder();
}

void PageInterface::drawBorder()
{
	QPen p;
	p.setStyle(Qt::DashDotDotLine);

	delete m_paperBorder;
	delete m_pageBorder;

	m_paperBorder = addRect(m_paperRect);
	p.setStyle(Qt::SolidLine);
	p.setColor(QColor(225,224,224));
	p.setWidth(2);
	m_pageBorder = addRect(geometry());

	m_pageBorder->setPen(p);
	if (m_showGrid)
	{
		QPen p;
		p.setColor(QColor(150,150,255));
		p.setWidth(1);
		if (views().size())
			p.setWidth(1+1/views()[0]->transform().m11());

		for (int i=0;i<width();i+=(5./UNIT))
		{
			QGraphicsLineItem * line= new QGraphicsLineItem(m_paperBorder);
			line->setPen(p);
			line->setLine(i,0,i,height());
		}
		for (int i=0;i<height();i+=(5./UNIT))
		{
			QGraphicsLineItem * line= new QGraphicsLineItem(m_paperBorder);
			line->setPen(p);
			line->setLine(0,i,width(),i);
		}
	}
	m_pageBorder->setZValue(-1) ;
	m_paperBorder->setZValue(-2) ;
}

}
