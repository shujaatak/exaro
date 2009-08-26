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
#include "itemstoolbar.h"
#include <QTimer>

#define ICONSDIR "../share/icons/"

ItemsToolBar::ItemsToolBar(mainWindow * mw)
	:QToolBar( mw )
	,m_menu(0)
{
    m_mw = mw;
    this->setObjectName("ItemsToolBar");
    this->setWindowTitle(tr ("Items ToolBar"));
    QString category;
    bool allIconsExists = true;
    for ( int i = 0;i < m_mw->m_reportEngine.items().uniqueKeys().size();i++ )
    {
	category = m_mw->m_reportEngine.items().uniqueKeys()[i];
	QIcon icon(QCoreApplication::applicationDirPath () + "/" + ICONSDIR + "/exaro_" + category + ".png");

	QAction * newItem = new QAction(icon, category, this );

	if (icon.isNull())
	    allIconsExists = false;

	addAction( newItem );

	Menu * menu = new Menu( this );
	for ( int j = 0;j < m_mw->m_reportEngine.items().values( m_mw->m_reportEngine.items().uniqueKeys()[i] ).count();j++ )
	{
	    menu->addAction( m_mw->m_reportEngine.items().values( m_mw->m_reportEngine.items().uniqueKeys()[i] )[j]->toolBoxIcon(), m_mw->m_reportEngine.items().values( m_mw->m_reportEngine.items().uniqueKeys()[i] )[j]->metaObject()->className());
	}

	menus[newItem->text()] = menu;
	connect ( newItem, SIGNAL(hovered()), this, SLOT(showItems()) );
	connect ( newItem, SIGNAL(triggered()), this, SLOT(showItems()) );
    }

    if (!allIconsExists)
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

}

void ItemsToolBar::showItems()
{
    if (m_menu)
	m_menu->hide();
    m_menu = menus[((QAction*)sender())->text()];

    if (orientation () == Qt::Vertical)
	m_menu->popup(  mapToGlobal( QPoint(this->geometry().width() , widgetForAction((QAction*)sender())->pos().y()) ) );
    else
	m_menu->popup(  mapToGlobal( QPoint(widgetForAction((QAction*)sender())->pos().x(), this->geometry().height()) ) );
}

void ItemsToolBar::leaveEvent ( QEvent * event )
{
    QToolBar::leaveEvent(event);
    QTimer::singleShot(100,this,SLOT(tryHideMenu()));
}

void ItemsToolBar::tryHideMenu()
{
    if ((m_menu) && (!m_menu->underMouse()))
	m_menu->hide();
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
