/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2009 by Mikhalov Alexaner                               *
 *   alexmi3@rambler.ru                                                    *
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

#include "itemselection.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QGraphicsSceneMouseEvent>
#include "grid.h"

#define BOUND 0
#define SIZE 20

// ----------- ItemHandle
ItemHandle::ItemHandle(Type t, ItemSelection *s) :
    QGraphicsItem (s),
    m_item(0),
    m_type(t),
    m_sel(s),
    m_active(true)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    updateCursor();
}

void ItemHandle::updateCursor()
{
    if (!m_active) {
        setCursor(Qt::ArrowCursor);
        return;
    }

    switch (m_type) {
    case LeftTop:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case Top:
        setCursor(Qt::SizeVerCursor);
        break;
    case RightTop:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case Right:
        setCursor(Qt::SizeHorCursor);
        break;
    case RightBottom:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case LeftBottom:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case Left:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        Q_ASSERT(0);
    }
}

void ItemHandle::setActive(bool a)
{
    m_active = a;
    updateCursor();
}

void ItemHandle::setItem(Report::ItemInterface *i)
{
    m_item = i;
}

void ItemHandle::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->setPen( Qt::black);
//    painter->drawRect(0, 0, SIZE - 1, SIZE - 1);
    painter->fillRect( 0, 0, SIZE - 1, SIZE - 1, QBrush(Qt::blue) );
}

QRectF ItemHandle::boundingRect () const
{
    return QRectF(0,0,SIZE,SIZE);
}

void ItemHandle::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mousePressEvent(e );

    if (!(m_item && e->button() == Qt::LeftButton))
        return;

    if (!(m_active))
        return;

    m_origPressPos = e->scenePos();
    m_geom = m_origGeom = QRectF (m_item->scenePos(), m_item->boundingRect().size());
}

void ItemHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mouseMoveEvent( e );
    if (!(m_item && m_active && e->buttons() & Qt::LeftButton))
        return;

    const QPointF rp = e->scenePos();
    const QPointF d = rp - m_origPressPos;

//    const QRect pr = container->rect();
//    const QRectF pr = m_item->boundingRect();

    switch (m_type) {

    case LeftTop: {
//	if (rp.x() > pr.width() - 2 * SIZE || rp.y() > pr.height() - 2 * SIZE)
//            return;
        int w = m_origGeom.width() - d.x();
	m_geom.setWidth(w);
	w = Grid::instance()->adjustX(w);

        int h = m_origGeom.height() - d.y();
	m_geom.setHeight(h);
	h = Grid::instance()->adjustY(h);

	const int dx = m_item->width() - w;
	const int dy = m_item->height() - h;

	trySetGeometry(m_item, m_item->x() + dx, m_item->y() + dy, w, h);
    } break;

    case Top: {
//        if (rp.y() > pr.height() - 2 * height())
//            return;

        int h = m_origGeom.height() - d.y();
        m_geom.setHeight(h);
//        h = grid.widgetHandleAdjustY(h);
	h = Grid::instance()->adjustY(h);

	const int dy = m_item->height() - h;
	trySetGeometry(m_item, m_item->x(), m_item->y() + dy, m_item->width(), h);
    } break;

    case RightTop: {
//        if (rp.x() < 2 * width() || rp.y() > pr.height() - 2 * height())
//            return;

        int h = m_origGeom.height() - d.y();
        m_geom.setHeight(h);
//        h = grid.widgetHandleAdjustY(h);
	h = Grid::instance()->adjustY(h);

	const int dy = m_item->height() - h;

        int w = m_origGeom.width() + d.x();
        m_geom.setWidth(w);
//        w = grid.widgetHandleAdjustX(w);
	w = Grid::instance()->adjustX(w);

	trySetGeometry(m_item, m_item->x(), m_item->y() + dy, w, h);
    } break;

    case Right: {
//        if (rp.x() < 2 * width())
//            return;

        int w = m_origGeom.width() + d.x();
        m_geom.setWidth(w);
//        w = grid.widgetHandleAdjustX(w);
	w = Grid::instance()->adjustX(w);

	tryResize(m_item, w, m_item->height());
    } break;

    case RightBottom: {
//        if (rp.x() < 2 * width() || rp.y() < 2 * height())
//            return;

        int w = m_origGeom.width() + d.x();
        m_geom.setWidth(w);
//        w = grid.widgetHandleAdjustX(w);
	w = Grid::instance()->adjustX(w);

        int h = m_origGeom.height() + d.y();
        m_geom.setHeight(h);
//        h = grid.widgetHandleAdjustY(h);
	h = Grid::instance()->adjustY(h);

	tryResize(m_item, w, h);
    } break;

    case Bottom: {
//        if (rp.y() < 2 * height())
//            return;

        int h = m_origGeom.height() + d.y();
        m_geom.setHeight(h);
//        h = grid.widgetHandleAdjustY(h);
	h = Grid::instance()->adjustY(h);

	tryResize(m_item, m_item->width(), h);
    } break;

    case LeftBottom: {
//        if (rp.x() > pr.width() - 2 * width() || rp.y() < 2 * height())
//            return;

        int w = m_origGeom.width() - d.x();
        m_geom.setWidth(w);
//        w = grid.widgetHandleAdjustX(w);
	w = Grid::instance()->adjustX( w );

        int h = m_origGeom.height() + d.y();
        m_geom.setHeight(h);
//        h = grid.widgetHandleAdjustY(h);
	h = Grid::instance()->adjustY(h);

	int dx = m_item->width() - w;

	trySetGeometry(m_item, m_item->x() + dx, m_item->y(), w, h);
    } break;

    case Left: {
//        if (rp.x() > pr.width() - 2 * width())
//            return;

        int w = m_origGeom.width() - d.x();
        m_geom.setWidth(w);
//        w = grid.widgetHandleAdjustX(w);
	w = Grid::instance()->adjustX( w );

	const int dx = m_item->width() - w;

	trySetGeometry(m_item, m_item->x() + dx, m_item->y(), w, m_item->height());
    } break;

    default: break;

    } // end switch

    m_sel->updateGeometry();
/*
    if (LayoutInfo::layoutType(m_formWindow->core(), m_widget) != LayoutInfo::NoLayout)
        m_formWindow->updateChildSelections(m_widget);
	*/
}

void ItemHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mouseReleaseEvent( e );
    if (e->button() != Qt::LeftButton || !m_active)
        return;


//    if (!m_formWindow->hasFeature(FormWindow::EditFeature))
//        return;

    /*
    switch (ItemSelection::widgetState(m_formWindow->core(), m_widget)) {
    case ItemSelection::UnlaidOut:
       if (m_geom != m_widget->geometry()) {
           SetPropertyCommand *cmd = new SetPropertyCommand(m_formWindow);
           cmd->init(m_widget, QLatin1String("geometry"), m_widget->geometry());
           cmd->setOldValue(m_origGeom);
           m_formWindow->commandHistory()->push(cmd);
           m_formWindow->emitSelectionChanged();
       }
       break;
    case ItemSelection::LaidOut:
       break;
    case ItemSelection::ManagedGridLayout:
       changeGridLayoutItemSpan();
       break;
    case ItemSelection::ManagedFormLayout:
        changeFormLayoutItemSpan();
        break;
    }
    */
}

/*
// Match the left/right widget handle mouse movements to form layout span-changing operations
static inline int formLayoutLeftHandleOperation(int dx, unsigned possibleOperations)
{
    if (dx < 0) {
        if (possibleOperations & ChangeFormLayoutItemRoleCommand::FieldToSpanning)
            return ChangeFormLayoutItemRoleCommand::FieldToSpanning;
        return 0;
    }
    if (possibleOperations & ChangeFormLayoutItemRoleCommand::SpanningToField)
        return ChangeFormLayoutItemRoleCommand::SpanningToField;
    return 0;
}

static inline int formLayoutRightHandleOperation(int dx, unsigned possibleOperations)
{
    if (dx < 0) {
        if (possibleOperations & ChangeFormLayoutItemRoleCommand::SpanningToLabel)
            return ChangeFormLayoutItemRoleCommand::SpanningToLabel;
        return 0;
    }
    if (possibleOperations & ChangeFormLayoutItemRoleCommand::LabelToSpanning)
        return ChangeFormLayoutItemRoleCommand::LabelToSpanning;
    return 0;
}

// Change form layout item horizontal span
void ItemHandle::changeFormLayoutItemSpan()
{
    QUndoCommand *cmd = 0;
    // Figure out command according to the movement
    const int dx = m_widget->geometry().center().x() - m_origGeom.center().x();
    if (qAbs(dx) >= QApplication::startDragDistance()) {
        int operation = 0;
        if (const unsigned possibleOperations = ChangeFormLayoutItemRoleCommand::possibleOperations(m_formWindow->core(), m_widget)) {
            switch (m_type) {
	    case ItemHandle::Left:
                operation = formLayoutLeftHandleOperation(dx, possibleOperations);
                break;
	    case ItemHandle::Right:
                operation = formLayoutRightHandleOperation(dx, possibleOperations);
                break;
            default:
                break;
            }
            if (operation) {
                ChangeFormLayoutItemRoleCommand *fcmd = new ChangeFormLayoutItemRoleCommand(m_formWindow);
                fcmd->init(m_widget, static_cast<ChangeFormLayoutItemRoleCommand::Operation>(operation));
                cmd = fcmd;
            }
        }
    }
    if (cmd) {
        m_formWindow->commandHistory()->push(cmd);
    } else {
        // Cancelled/Invalid. Restore the size of the widget.
        if (QFormLayout *form = managedLayoutOf<QFormLayout>(m_formWindow->core(), m_widget)) {
            form->invalidate();
            form->activate();
            m_formWindow->clearSelection(false);
            m_formWindow->selectWidget(m_widget);
        }
    }
}
*/

/*
void ItemHandle::changeGridLayoutItemSpan()
{
    QDesignerLayoutDecorationExtension *deco = qt_extension<QDesignerLayoutDecorationExtension*>(core()->extensionManager(), m_widget->parentWidget());
    if (!deco)
       return;
    QGridLayout *grid = managedLayoutOf<QGridLayout>(m_formWindow->core(), m_widget);
    if (!grid)
       return;

    const QSize size = m_widget->parentWidget()->size();

    const int index = deco->indexOf(m_widget);
    const QRect info = deco->itemInfo(index);
    const int top = deco->findItemAt(info.top() - 1, info.left());
    const int left = deco->findItemAt(info.top(), info.left() - 1);
    const int bottom = deco->findItemAt(info.bottom() + 1, info.left());
    const int right = deco->findItemAt(info.top(), info.right() + 1);

    const QPoint pt = m_origGeom.center() - m_widget->geometry().center();

    ChangeLayoutItemGeometry *cmd = 0;

    switch (m_type) {
    default:
       break;

    case ItemHandle::Top: {
       if (pt.y() < 0 && info.height() > 1) {
           cmd = new ChangeLayoutItemGeometry(m_formWindow);
           cmd->init(m_widget, info.y() + 1, info.x(), info.height() - 1, info.width());
       } else if (pt.y() > 0 && top != -1 && grid->itemAt(top)->spacerItem()) {
           cmd = new ChangeLayoutItemGeometry(m_formWindow);
           cmd->init(m_widget, info.y() - 1, info.x(), info.height() + 1, info.width());
       }
    }
       break;

    case ItemHandle::Left: {
       if (pt.x() < 0 && info.width() > 1) {
           cmd = new ChangeLayoutItemGeometry(m_formWindow);
           cmd->init(m_widget, info.y(), info.x() + 1, info.height(), info.width() - 1);
       } else if (pt.x() > 0 && left != -1 && grid->itemAt(left)->spacerItem()) {
           cmd = new ChangeLayoutItemGeometry(m_formWindow);
           cmd->init(m_widget, info.y(), info.x() - 1, info.height(), info.width() + 1);
       }
    }
       break;

    case ItemHandle::Right: {
       if (pt.x() > 0 && info.width() > 1) {
           cmd = new ChangeLayoutItemGeometry(m_formWindow);
           cmd->init(m_widget, info.y(), info.x(), info.height(), info.width() - 1);
       } else if (pt.x() < 0 && right != -1 && grid->itemAt(right)->spacerItem()) {
           cmd = new ChangeLayoutItemGeometry(m_formWindow);
           cmd->init(m_widget, info.y(), info.x(), info.height(), info.width() + 1);
       }
    }
       break;

    case ItemHandle::Bottom: {
       if (pt.y() > 0 && info.width() > 1) {
           cmd = new ChangeLayoutItemGeometry(m_formWindow);
           cmd->init(m_widget, info.y(), info.x(), info.height() - 1, info.width());
       } else if (pt.y() < 0 && bottom != -1 && grid->itemAt(bottom)->spacerItem()) {
           cmd = new ChangeLayoutItemGeometry(m_formWindow);
           cmd->init(m_widget, info.y(), info.x(), info.height() + 1, info.width());
       }
    }
       break;
    }

    if (cmd != 0) {
       m_formWindow->commandHistory()->push(cmd);
    } else {
       grid->invalidate();
       grid->activate();
       m_formWindow->clearSelection(false);
       m_formWindow->selectWidget(m_widget);
    }
}
*/

void ItemHandle::trySetGeometry(Report::ItemInterface *i, int x, int y, int width, int height)
{
    qDebug("trySetGeometry(%s, %i, %i, %i, %i)", qPrintable(i->objectName()), x,y,width, height);

    if  ( (width < SIZE*2) ||  (height<SIZE*2) )
	return;

    m_item->setGeometry( QRectF(x, y, width, height) );
//    if (!m_formWindow->hasFeature(FormWindow::EditFeature))
//        return;
/*
    int minw = w->minimumSize().width();
    minw = qMax(minw, 2 * m_formWindow->grid().x());

    int minh = w->minimumSize().height();
    minh = qMax(minh, 2 * m_formWindow->grid().y());

    if (qMax(minw, width) > w->maximumWidth() ||
         qMax(minh, height) > w->maximumHeight())
        return;

    if (width < minw && x != w->x())
        x -= minw - width;

    if (height < minh && y != w->y())
        y -= minh - height;

    w->setGeometry(x, y, qMax(minw, width), qMax(minh, height));
    */
}

void ItemHandle::tryResize(Report::ItemInterface *i, int width, int height)
{
    if  ( (width < SIZE*2) ||  (height<SIZE*2) )
	return;
    m_item->setWidth( width );
    m_item->setHeight( height );

    /*
    int minw = w->minimumSize().width();
    minw = qMax(minw, 16);

    int minh = w->minimumSize().height();
    minh = qMax(minh, 16);

    w->resize(qMax(minw, width), qMax(minh, height));
    */
}

// ------------------ ItemSelection

ItemSelection::ItemSelection():
    m_item(0),
//    m_guideItem(0),
    m_boundingRect(QRect()),
    QGraphicsItem()
{
//    setFlags(/*QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable*/0);
    setAcceptedMouseButtons(0);
    for (int i = ItemHandle::LeftTop; i < ItemHandle::TypeCount; ++i)
	m_handles[i] = new ItemHandle(static_cast<ItemHandle::Type>(i), this);

    setFlag(QGraphicsItem::ItemClipsToShape, true);
    hide();
}

void ItemSelection::setItem(Report::ItemInterface *i)
{
    if (m_item != 0)
	m_item->removeSceneEventFilter(this);

    if (i == 0) {
        hide();
	m_item = 0;
        return;
    }

    m_item = i;

    m_item->installSceneEventFilter(this);

    updateActive();

    updateGeometry();
    show();
}

void ItemSelection::updateActive()
{
//    const ItemState is = itemState(m_item);
    bool active[ItemHandle::TypeCount];
    qFill(active, active + ItemHandle::TypeCount, true);
    /*
    qFill(active, active + ItemHandle::TypeCount, false);
    // Determine active handles
    switch (is) {
    case UnlaidOut:
	qFill(active, active + ItemHandle::TypeCount, true);
        break;
    case ManagedGridLayout: // Grid: Allow changing span
	active[ItemHandle::Left] = active[ItemHandle::Top] = active[ItemHandle::Right] = active[ItemHandle::Bottom] = true;
        break;
    case ManagedFormLayout:  // Form: Allow changing column span
        if (const unsigned operation = ChangeFormLayoutItemRoleCommand::possibleOperations(m_formWindow->core(), m_widget)) {
	    active[ItemHandle::Left]  = operation & (ChangeFormLayoutItemRoleCommand::SpanningToField|ChangeFormLayoutItemRoleCommand::FieldToSpanning);
	    active[ItemHandle::Right] = operation & (ChangeFormLayoutItemRoleCommand::SpanningToLabel|ChangeFormLayoutItemRoleCommand::LabelToSpanning);
        }
        break;
    default:
        break;
    }
*/
    for (int i = ItemHandle::LeftTop; i < ItemHandle::TypeCount; ++i)
	if (ItemHandle *h = m_handles[i]) {
	    h->setItem(m_item);
            h->setActive(active[i]);
        }
}

bool ItemSelection::isUsed() const
{
    return m_item != 0;
}

void ItemSelection::updateGeometry()
{
    if (!m_item)
        return;

    QPointF p = m_item->scenePos();
    setPos(QPointF ( p.x() - BOUND, p.y() - BOUND ) );

    prepareGeometryChange();
    m_boundingRect = QRectF(0 ,0 , m_item->boundingRect().width() + 2*BOUND, m_item->boundingRect().height() + 2*BOUND);

    const QRectF r(QPointF(BOUND,BOUND), m_item->boundingRect().size());

    const int w = SIZE;
    const int h = SIZE;

    for (int i = ItemHandle::LeftTop; i < ItemHandle::TypeCount; ++i) {
	ItemHandle *hndl = m_handles[ i ];
        if (!hndl)
            continue;
        switch (i) {
	case ItemHandle::LeftTop:
	    hndl->setPos(r.x() - w / 2, r.y() - h / 2);
            break;
	case ItemHandle::Top:
	    hndl->setPos(r.x() + r.width() / 2 - w / 2, r.y() - h / 2);
            break;
	case ItemHandle::RightTop:
	    hndl->setPos(r.x() + r.width() - w / 2, r.y() - h / 2);
            break;
	case ItemHandle::Right:
	    hndl->setPos(r.x() + r.width() - w / 2, r.y() + r.height() / 2 - h / 2);
            break;
	case ItemHandle::RightBottom:
	    hndl->setPos(r.x() + r.width() - w / 2, r.y() + r.height() - h / 2);
            break;
	case ItemHandle::Bottom:
	    hndl->setPos(r.x() + r.width() / 2 - w / 2, r.y() + r.height() - h / 2);
            break;
	case ItemHandle::LeftBottom:
	    hndl->setPos(r.x() - w / 2, r.y() + r.height() - h / 2);
            break;
	case ItemHandle::Left:
	    hndl->setPos(r.x() - w / 2, r.y() + r.height() / 2 - h / 2);
            break;
        default:
            break;
        }
    }
}

QRectF ItemSelection::boundingRect () const
{
    return m_boundingRect;
}

void ItemSelection::hide()
{
    for (int i = ItemHandle::LeftTop; i < ItemHandle::TypeCount; ++i) {
	ItemHandle *h = m_handles[ i ];
        if (h)
            h->hide();
    }
}

void ItemSelection::show()
{
    for (int i = ItemHandle::LeftTop; i < ItemHandle::TypeCount; ++i) {
	ItemHandle *h = m_handles[ i ];
        if (h) {
            h->show();
        }
    }
}

void ItemSelection::update()
{
    for (int i = ItemHandle::LeftTop; i < ItemHandle::TypeCount; ++i) {
	ItemHandle *h = m_handles[ i ];
        if (h)
            h->update();
    }
}

Report::ItemInterface *ItemSelection::item() const
{
    return m_item;
}


void ItemSelection::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    m_item->paint(painter, option, widget);
    foreach (QGraphicsItem * item, m_item->childItems())
    {
	painter->save();
	painter->translate( item->pos() );
	item->paint(painter, option, widget);
	painter->restore();
    }
    painter->restore();
//
//    painter->save();
//    painter->setOpacity( 0.1 );
//    painter->fillRect( this->boundingRect(), QBrush(Qt::red));
//    painter->restore();
}

