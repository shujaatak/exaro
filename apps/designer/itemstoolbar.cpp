#include "itemstoolbar.h"

ItemsToolBar::ItemsToolBar(mainWindow * mw)
	:QToolBar( mw )
	,m_menu(0)
{
    m_mw = mw;
    this->setObjectName("ItemsToolBar");

    for ( int i = 0;i < m_mw->m_reportEngine.items().uniqueKeys().size();i++ )
    {
	QAction * newItem = new QAction(/*QIcon(":/images/edit.png"),*/ m_mw->m_reportEngine.items().uniqueKeys()[i], this );
	addAction( newItem );
	Menu * menu = new Menu( this );
	for ( int j = 0;j < m_mw->m_reportEngine.items().values( m_mw->m_reportEngine.items().uniqueKeys()[i] ).count();j++ )
	{
	    menu->addAction( m_mw->m_reportEngine.items().values( m_mw->m_reportEngine.items().uniqueKeys()[i] )[j]->toolBoxIcon(), m_mw->m_reportEngine.items().values( m_mw->m_reportEngine.items().uniqueKeys()[i] )[j]->metaObject()->className());
	}

	menus[newItem->text()] = menu;
	connect ( newItem, SIGNAL(hovered()), this, SLOT(showItems()) );
    }

}

void ItemsToolBar::showItems()
{
    if (m_menu)
	m_menu->hide();
    m_menu = menus[((QAction*)sender())->text()];

    m_menu->popup(  mapToGlobal( QPoint(this->geometry().width() - 5, widgetForAction((QAction*)sender())->pos().y()) ) );
}


//-------------Menu
Menu::Menu(QWidget * parent )
	:QMenu(parent)
{
    this->setAcceptDrops( true );
}

void Menu::leaveEvent ( QEvent * event )
{
    hide ();
}

void Menu::mousePressEvent ( QMouseEvent * event )
{
    QWidget::mousePressEvent( event );
    QAction * action = actionAt ( event->pos());
    if (!action)
	return;
//    qDebug("mouse pos = %i x %i", event->pos().x(), event->pos().y());
//    qDebug("item = %s", qPrintable((actionAt ( event->pos()))->text()) );

    if (event->button() == Qt::LeftButton) {

	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;

	mimeData->setText(QString("Exaro::%1").arg(action->text()));
	drag->setMimeData(mimeData);
	drag->setPixmap(action->icon().pixmap(100,100));

	Qt::DropAction dropAction = drag->exec();
    }
}

void Menu::dragEnterEvent(QDragEnterEvent *event)
{
    QMenu::dragEnterEvent( event );
    event->acceptProposedAction();
}

void Menu::dragLeaveEvent ( QDragLeaveEvent * /*event*/ )
{
    hide();
}
