/***************************************************************************
 *   Copyright (C) 2009 by Alexander Mikhalov                              *
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

#include "command.h"
#include <QGraphicsScene>
#include <QGraphicsView>


AddCommand::AddCommand( Report::PageInterface* page, const char* itemClassName, QPointF pos, mainWindow* mw )
{
	this->mw = mw;
	this->m_pos = pos;
	this->m_itemClassName = itemClassName;
	this->pageName = mw->m_tw->tabText( mw->m_tw->currentIndex() );

	setText( QObject::tr( "Add %1" )
	                .arg( createCommandString( itemClassName, m_pos ) ) );
}


void AddCommand::redo()
{
	qDebug( "AddCommand::redo()" );

	Report::PageInterface* m_page = ( Report::PageInterface* )dynamic_cast<QGraphicsView *>( findObjectByTabName( mw->m_tw, pageName ) )->scene();
	Q_ASSERT( m_page );

	QObject * m_parent = dynamic_cast<Report::ItemInterface*>( m_page->itemAt( m_pos ) ) ? dynamic_cast<QObject*>( m_page->itemAt( m_pos ) )
	                : dynamic_cast<Report::PageInterface*>( m_page );
	Report::ItemInterface* itemExample = mw->m_reportEngine.findItemByClassName( m_itemClassName );

	Report::ItemInterface *m_item = 0;

	if ( dynamic_cast<Report::ItemInterface*>( m_parent ) )
	{
		if ( dynamic_cast<Report::ItemInterface*>( m_parent )->canContain( itemExample ) )
			m_item = dynamic_cast<Report::ItemInterface*>( itemExample->createInstance( dynamic_cast<QGraphicsItem*>( m_parent ), m_parent ) );
	}
	else
		if ( dynamic_cast<Report::PageInterface*>( m_parent )->canContain( itemExample ) )
			dynamic_cast<Report::PageInterface*>( m_parent )->addItem( m_item = dynamic_cast<Report::ItemInterface*>( itemExample->createInstance( 0, m_parent ) ) );

	if ( m_item )
	{
		itemName = Report::ReportEngine::uniqueName( m_item->metaObject()->className(), mw->m_report );
		m_item->setObjectName( itemName );

		connect( m_item, SIGNAL( itemSelected( QObject*, QPointF ) ), mw, SLOT( itemSelected( QObject*, QPointF ) ) );
		connect( m_item, SIGNAL( geometryChanged( QObject*, QRectF, QRectF ) ), mw, SLOT( itemGeometryChanged( QObject*, QRectF, QRectF ) ) );


		if ( dynamic_cast<Report::BandInterface*>( m_item ) )
			dynamic_cast<Report::BandInterface*>( m_item )->setOrder( INT_MAX );

		QPointF localPos = m_item->mapFromScene( m_pos );
		m_item->setGeometry( QRectF( localPos.x(), localPos.y(), m_item->geometry().width(), m_item->geometry().height() ) );
		mw->m_pe->setObject( m_item );
		mw->m_objectModel.setRootObject( mw->m_report );
		mw->selectObject( m_item, mw->m_objectModel.index( 0, 0 ) );

		itemName = m_item->objectName();
	}
	else
	{
		mw->m_pe->setObject( m_parent );
		mw->selectObject( m_parent, mw->m_objectModel.index( 0, 0 ) );
	}
}


void AddCommand::undo()
{
	Report::PageInterface* m_page = ( Report::PageInterface* )dynamic_cast<QGraphicsView *>( findObjectByTabName( mw->m_tw, pageName ) )->scene();
	Q_ASSERT( m_page );

	Report::ItemInterface *m_item = dynamic_cast<Report::ItemInterface *>( findObject( m_page, itemName ) );

	if ( !m_item )
		return;

	QObject * parent = dynamic_cast<QObject*>( m_item )->parent();

	m_page->removeItem( m_item );

	m_item->setParentItem( 0 );
	dynamic_cast<Report::ItemInterface*>( m_item )->removeItem();
	delete dynamic_cast<Report::ItemInterface*>( m_item );
	mw->m_lastSelectedObject = parent;
	mw->m_pe->setObject( parent );
	mw->m_objectModel.setRootObject( mw->m_report );
	mw->selectObject( parent, mw->m_objectModel.index( 0, 0 ) );
	m_item = 0;
}



MoveCommand::MoveCommand( Report::ItemInterface *item, const QPointF &oldPos, mainWindow* mw )
{
	this->mw = mw;
	this->m_newPos = dynamic_cast<Report::ItemInterface *>( item )->pos();
	this->m_oldPos = oldPos;
	this->itemName = item->objectName();
	this->pageName = mw->m_tw->tabText( mw->m_tw->currentIndex() );

	const char * itemClassName = dynamic_cast<Report::ItemInterface*>( item )->metaObject()->className();
	setText( QObject::tr( "Move %1" )
	                .arg( createCommandString( itemClassName, m_newPos ) ) );
}

void MoveCommand::redo()
{
	Report::PageInterface* m_page = ( Report::PageInterface* )dynamic_cast<QGraphicsView *>( findObjectByTabName( mw->m_tw, pageName ) )->scene();
	Q_ASSERT( m_page );

	Report::ItemInterface *m_item = dynamic_cast<Report::ItemInterface *>( findObject( m_page, itemName ) );

	if ( !m_item )
		return;

	dynamic_cast<Report::ItemInterface*>( m_item )->setPos( m_newPos );
}

void MoveCommand::undo()
{
	Report::PageInterface* m_page = ( Report::PageInterface* )dynamic_cast<QGraphicsView *>( findObjectByTabName( mw->m_tw, pageName ) )->scene();
	Q_ASSERT( m_page );

	Report::ItemInterface *m_item = dynamic_cast<Report::ItemInterface *>( findObject( m_page, itemName ) );

	if ( !m_item )
		return;
	dynamic_cast<Report::ItemInterface*>( m_item )->setPos( m_oldPos );
}


DelCommand::DelCommand( Report::ItemInterface* item, mainWindow* mw )
{
	this->mw = mw;
	this->parentName = dynamic_cast<Report::ItemInterface*>( item->parent() ) ? dynamic_cast<Report::ItemInterface*>( item->parent() )->objectName() : QString();
	this->itemName = item->objectName();
//    this->m_page = dynamic_cast<Report::PageInterface*>((item)->scene());
	this->pageName = mw->m_tw->tabText( mw->m_tw->currentIndex() );

	QDomDocument doc;
	doc.appendChild( mw->m_reportEngine.objectProperties(( QObject * )item, &doc ) );
	domObject = doc.toString( 0 );

	const char* itemClassName = item->metaObject()->className();
	setText( QObject::tr( "Delete %1" )
	                .arg( createCommandString( itemClassName, item->pos() ) ) );
}


void DelCommand::redo()
{
	Report::PageInterface* m_page = ( Report::PageInterface* )dynamic_cast<QGraphicsView *>( findObjectByTabName( mw->m_tw, pageName ) )->scene();
	Q_ASSERT( m_page );

	QObject *m_parent;

	if ( parentName.isNull() )
		m_parent = dynamic_cast<QObject*>( m_page );
	else
		m_parent = findObject( m_page, parentName );

	Report::ItemInterface *m_item = dynamic_cast<Report::ItemInterface *>( findObject( m_page, itemName ) );



	if ( !m_item || !m_parent )
		return;

	if ( m_page )
		m_page->removeItem( m_item );

	m_item->setParentItem( 0 );
	dynamic_cast<Report::ItemInterface*>( m_item )->removeItem();
	delete dynamic_cast<Report::ItemInterface*>( m_item );
	mw->m_lastSelectedObject = m_parent;
	mw->m_pe->setObject( m_parent );
	mw->m_objectModel.setRootObject( mw->m_report );
	mw->selectObject( m_parent, mw->m_objectModel.index( 0, 0 ) );
}

void DelCommand::undo()
{
	Report::PageInterface* m_page = ( Report::PageInterface* )dynamic_cast<QGraphicsView *>( findObjectByTabName( mw->m_tw, pageName ) )->scene();
	Q_ASSERT( m_page );

	QObject *m_parent;
	if ( parentName.isNull() )
		m_parent = m_page;
	else
		m_parent = findObject( m_page, parentName );

	Report::ItemInterface *m_item;

	if ( !m_parent )
		return;

	QDomDocument doc;
	doc.setContent( domObject );
	QObject * obj = mw->m_reportEngine.objectFromDom( m_parent, doc.firstChildElement() );

	if ( dynamic_cast<Report::BandInterface*>( obj ) )
	{
		dynamic_cast<Report::BandInterface*>( obj )->setOrder( INT_MAX );
		dynamic_cast<Report::BandInterface*>( obj )->setGeometry( QRectF( 0, 0, dynamic_cast<Report::BandInterface*>( obj )->geometry().width(), dynamic_cast<Report::BandInterface*>( obj )->geometry().height() ) );
	}

	m_item = dynamic_cast<Report::ItemInterface*>( obj );

	if ( dynamic_cast<Report::ItemInterface*>( m_parent ) )
	{
		if ( !dynamic_cast<Report::ItemInterface*>( m_parent )->canContain( m_item ) )
		{
			delete m_item;
			m_item = 0;
		}
	}
	else
		if ( dynamic_cast<Report::PageInterface*>( m_parent )->canContain( m_item ) )
			dynamic_cast<Report::PageInterface*>( m_parent )->addItem( m_item );
		else
		{
			delete m_item;
			m_item = 0;
		}

	if ( m_item )
	{
		connect( m_item, SIGNAL( itemSelected( QObject *, QPointF ) ), mw, SLOT( itemSelected( QObject *, QPointF ) ) );
		connect( obj, SIGNAL( geometryChanged( QObject*, QRectF, QRectF ) ), mw, SLOT( itemGeometryChanged( QObject*, QRectF, QRectF ) ) );
		foreach( QObject * obj, ( dynamic_cast<QObject *>( m_item ) )->children() )
		{
			connect( obj, SIGNAL( itemSelected( QObject *, QPointF ) ), mw, SLOT( itemSelected( QObject *, QPointF ) ) );
			connect( obj, SIGNAL( geometryChanged( QObject*, QRectF, QRectF ) ), mw, SLOT( itemGeometryChanged( QObject*, QRectF, QRectF ) ) );
		}
		if ( dynamic_cast<Report::BandInterface*>( m_item ) )
			dynamic_cast<Report::BandInterface*>( m_item )->setOrder( INT_MAX );

		mw->m_pe->setObject( m_item );
		mw->m_objectModel.setRootObject( mw->m_report );
		mw->selectObject( m_item, mw->m_objectModel.index( 0, 0 ) );
	}
}


PropertyChangeCommand::PropertyChangeCommand( QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value, mainWindow* mw )
{
	this->mw = mw;
	this->propertyName = propertyName;
	this->old_value = old_value;
	this->new_value = new_value;
	this->itemName = obj->objectName();
	this->pageName = mw->m_tw->tabText( mw->m_tw->currentIndex() );
	setText( QObject::tr( "'%1' property '%2'" ).arg( itemName ).arg( propertyName ) );
}

void PropertyChangeCommand::redo()
{
	Report::PageInterface* m_page = ( Report::PageInterface* )dynamic_cast<QGraphicsView *>( findObjectByTabName( mw->m_tw, pageName ) )->scene();
	Q_ASSERT( m_page );

	QObject *item;

	if ( m_page->objectName() == itemName )
		item = m_page;
	else
		item = findObject( m_page, itemName );

	if ( !item )
		return;
	if ( propertyName == "objectName" )
		this->itemName = new_value.toString();
	item->setProperty( qPrintable( propertyName ), new_value );
}


void PropertyChangeCommand::undo()
{
	Report::PageInterface* m_page = ( Report::PageInterface* )dynamic_cast<QGraphicsView *>( findObjectByTabName( mw->m_tw, pageName ) )->scene();
	Q_ASSERT( m_page );

	QObject *item;
	if ( m_page->objectName() == itemName )
		item = m_page;
	else
		item = findObject( m_page, itemName );

	if ( !item )
		return;

	if ( propertyName == "objectName" )
		this->itemName = old_value.toString();
	item->setProperty( qPrintable( propertyName ), old_value );
}


bool PropertyChangeCommand::mergeWith( const QUndoCommand *command )
{
	const PropertyChangeCommand *newCommand = static_cast<const PropertyChangeCommand *>( command );
	QString nName = newCommand->itemName;
	QString nProperty = newCommand->propertyName;
	QString nPageName = newCommand->pageName;

	if (( itemName != nName || propertyName != nProperty || pageName != nPageName ) && ( propertyName != "objectName" ) )
		return false;

	this->new_value = newCommand->new_value;
	if ( propertyName == "objectName" )
		this->itemName = new_value.toString();
	setText( QObject::tr( "'%1' property '%2'" ).arg( itemName ).arg( propertyName ) );
	return true;
}


GeometryChangeCommand::GeometryChangeCommand( QObject* obj, QRectF newGeometry, QRectF oldGeometry, mainWindow* mw )
{
	this->mw = mw;
	this->oldGeometry = oldGeometry;
	this->newGeometry = newGeometry;
	this->itemName = obj->objectName();
	this->pageName = mw->m_tw->tabText( mw->m_tw->currentIndex() );
	setText( QObject::tr( "'%1' change size '%2x%3'" ).arg( itemName ).arg( newGeometry.height() ).arg( newGeometry.width() ) );
}


void GeometryChangeCommand::redo()
{
	qDebug( "GeometryChangeCommand::redo()" );

	Report::PageInterface* m_page = ( Report::PageInterface* )dynamic_cast<QGraphicsView *>( findObjectByTabName( mw->m_tw, pageName ) )->scene();
	Q_ASSERT( m_page );

	QObject *item;

	if ( m_page->objectName() == itemName )
		item = m_page;
	else
		item = findObject( m_page, itemName );

	if ( !item )
		return;

	if ( dynamic_cast<Report::ItemInterface*>( item ) )
		dynamic_cast<Report::ItemInterface*>( item )->setGeometry( newGeometry );
	else
		dynamic_cast<Report::PageInterface*>( item )->setGeometry( newGeometry );
}


void GeometryChangeCommand::undo()
{
	Report::PageInterface* m_page = ( Report::PageInterface* )dynamic_cast<QGraphicsView *>( findObjectByTabName( mw->m_tw, pageName ) )->scene();
	Q_ASSERT( m_page );

	QObject *item;
	if ( m_page->objectName() == itemName )
		item = m_page;
	else
		item = findObject( m_page, itemName );

	if ( !item )
		return;

	if ( dynamic_cast<Report::ItemInterface*>( item ) )
		dynamic_cast<Report::ItemInterface*>( item )->setGeometry( oldGeometry );
	else
		dynamic_cast<Report::PageInterface*>( item )->setGeometry( oldGeometry );
}


bool GeometryChangeCommand::mergeWith( const QUndoCommand *command )
{
	const GeometryChangeCommand *newCommand = static_cast<const GeometryChangeCommand *>( command );
	QString nName = newCommand->itemName;
	QString nPageName = newCommand->pageName;

	if ( itemName != nName || pageName != nPageName )
		return false;

	this->newGeometry = newCommand->newGeometry;

	setText( QObject::tr( "'%1' change size '%2x%3'" ).arg( itemName ).arg( newGeometry.height() ).arg( newGeometry.width() ) );
	return true;
}


NewPageCommand::NewPageCommand( mainWindow * mw )
{
	this->mw = mw;
}


void NewPageCommand::redo()
{
	m_index = mw->_createNewPage_();
	setText( QObject::tr( "NewPage \'%1\'" ).arg( mw->m_tw->tabText( m_index ) ) );
}

void NewPageCommand::undo()
{
	mw->_deletePage_( m_index );
}


RemovePageCommand::RemovePageCommand( mainWindow * mw, int index )
{
	this->mw = mw;
//    this->m_index = index;
	this->pageName = mw->m_tw->tabText( index );
}

void RemovePageCommand::redo()
{
	int index = findIndexByTabName( mw->m_tw, pageName );
	mw->_deletePage_( index );

// FIXME: need Store page elements

	setText( QObject::tr( "Remove Page \'%1\'" ).arg( pageName ) );
}

void RemovePageCommand::undo()
{
	int index = mw->_createNewPage_();
	mw->m_tw->setTabText( index, pageName );
}

/*
ChangePageCommand::ChangePageCommand(mainWindow * mw, int newIndex, int oldIndex)
{
    this->mw = mw;
    this->newName = mw->m_tw->tabText(newIndex);
    this->oldName = mw->m_tw->tabText(oldIndex);
    this->fromStack = false;
    setText(QObject::tr("change page \'%1\'-> \'%2\'").arg(oldName).arg(newName));
}

void ChangePageCommand::redo()
{
    if (fromStack)
    {
        int index = findIndexByTabName(m_tw, newName);
        if (index >= 0)
            mw->m_tw->setCurrentIndex(index);
    }
    else
        fromStack = true;
}

void ChangePageCommand::undo()
{
    int index = findIndexByTabName(m_tw, oldName);
    if (index >= 0)
        mw->m_tw->setCurrentIndex(index);
}

*/

QString createCommandString( Report::ItemInterface  *item, const QPointF &pos )
{
	return QObject::tr( "%1 at (%2, %3)" )
	                .arg( item->metaObject()->className() )
	                .arg( pos.x() ).arg( pos.y() );
}

QString createCommandString( const char* name, const QPointF &pos )
{
	return QObject::tr( "%1 at (%2, %3)" )
	                .arg( name )
	                .arg( pos.x() ).arg( pos.y() );
}

QObject * findObject( Report::PageInterface * page, QString name )
{
	QList<QObject*> list = page->findChildren<QObject *>( name );
	if ( list.isEmpty() )
		return 0;

	return dynamic_cast<Report::ItemInterface *>( page->findChildren<QObject *>( name ).first() );
}


QWidget * findObjectByTabName( QTabWidget * tw, QString tabName )
{
	for ( int i = 0; tw->count(); i++ )
	{
		qDebug( "i=%i", i );
		if ( tw->tabText( i ) == tabName )
			return tw->widget( i );
	}
	return 0;

	/*
	QList<QObject*> list = tw->findChildren<QObject *>(tabName);
	if (list.isEmpty())
	    return 0;

	foreach (QObject* obj, list)
	{
	    if (!dynamic_cast<QGraphicsView *>(obj))
	        continue;
	    return dynamic_cast<QGraphicsView *>(obj);
	}
	return 0;
	*/
}

int findIndexByTabName( QTabWidget * tw, QString tabName )
{
	for ( int i = 0; tw->count(); i++ )
	{
		if ( tw->tabText( i ) == tabName )
			return i;
	}
	return -1;
}
