/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bogdan@licentia.eu                                                    *
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

#include <QDockWidget>
#include <QSettings>
#include <QDir>
#include <QGraphicsView>
#include <QToolBox>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QProcess>
#include <QTreeView>
#include <QItemSelectionModel>
#include <QMetaClassInfo>
#include <QUndoView>

#include <QPluginLoader>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDebug>

#include "mainwindow.h"
#include "designerpage.h"
#include "scripteditdialog.h"
#include "sqldatabasedialog.h"
#include "iteminterface.h"
#include "aboutdialog.h"
#include "optionsdialog.h"
#include "namevalidator.h"
#include "reportwizard.h"


#define ROWS_IN_MENU  10

#define screen_heightMM (((double)QDesktopWidget().screen()->height() /(double)QDesktopWidget().screen()->physicalDpiY() )*25.4)
#define screen_widthMM (((double)QDesktopWidget().screen()->width() /(double)QDesktopWidget().screen()->physicalDpiX() )*25.4)

#ifndef EXARO_VERSION
	#define EXARO_VERSION "0.0.0"
#endif

#ifndef LIB_INSTALL_DIR
	#define LIB_INSTALL_DIR "../lib"
#endif

mainWindow::mainWindow( QWidget* parent, Qt::WFlags fl )
		: QMainWindow( parent, fl ), m_report( 0 )
{
	setupUi( this );
	setWindowTitle( tr( "eXaro v%1 unsaved report" ).arg( EXARO_VERSION ) );

#ifdef Q_OS_MAC
	toolBar->setFloatable( false );
	setUnifiedTitleAndToolBarOnMac( true );
#endif

	m_tb = new QToolBox( this );
	m_dwToolBox = new QDockWidget( tr( "Tool Box" ) , this );
	m_dwToolBox->setObjectName( "Tool Box" );
	m_dwToolBox->setWidget( m_tb );
	m_dwToolBox->setAllowedAreas( Qt::AllDockWidgetAreas );
	addDockWidget( Qt::LeftDockWidgetArea, m_dwToolBox );
	m_pe = new PropertyEditor::PropertyEditor( this );
	m_dwPropertyEditor = new QDockWidget( tr( "Property Editor" ) , this );
	m_dwPropertyEditor->setObjectName( "Property Editor" );
	m_dwPropertyEditor->setWidget( m_pe );
	m_dwPropertyEditor->setAllowedAreas( Qt::AllDockWidgetAreas );
	addDockWidget( Qt::RightDockWidgetArea, m_dwPropertyEditor );
	m_nameValidator = new NameValidator( this );
	m_pe->setValidator( QVariant::String, m_nameValidator );

	m_dwQueryEditor = new QDockWidget( tr( "Query Editor" ), this );
	m_dquery = new Report::DesignerQueryWidget( m_dwQueryEditor );
	m_dwQueryEditor->setObjectName( "Query Editor" );
	m_dwQueryEditor->setWidget( m_dquery );
	m_dwQueryEditor->setAllowedAreas( Qt::AllDockWidgetAreas );
	addDockWidget( Qt::BottomDockWidgetArea, m_dwQueryEditor );

	m_dwUiEditor = new QDockWidget( tr( "Ui Editor" ), this );
	m_dui = new Report::DesignerUiWidget( m_dwUiEditor );
	m_dwUiEditor->setObjectName( "Ui Editor" );
	m_dwUiEditor->setWidget( m_dui );
	m_dwUiEditor->setAllowedAreas( Qt::AllDockWidgetAreas );
	addDockWidget( Qt::BottomDockWidgetArea, m_dwUiEditor );

	m_objectInspector = new QTreeView( this );
	m_objectInspector->setModel( &m_objectModel );
	m_objectInspector->setHeaderHidden( true );
	m_objectInspector->setSelectionMode( QAbstractItemView::SingleSelection );
	QItemSelectionModel *selectionModel = new QItemSelectionModel( &m_objectModel );
	m_objectInspector->setSelectionModel( selectionModel );

	m_dwObjectInspector = new QDockWidget( tr( "Object Inspector" ), this );
	m_dwObjectInspector->setObjectName( "Object Inspector" );
	m_dwObjectInspector->setWidget( m_objectInspector );
	m_dwObjectInspector->setAllowedAreas( Qt::AllDockWidgetAreas );
	addDockWidget( Qt::BottomDockWidgetArea, m_dwObjectInspector );

	m_undoStack = new QUndoStack( this );
	m_undoView = new QUndoView( m_undoStack );
	m_undoView->setWindowTitle( tr( "Command List" ) );

	m_lastUndoIndex=m_undoStack->index();

	m_dwUndoView = new QDockWidget( tr( "Command List" ), this );
	m_dwUndoView->setObjectName( "Command List" );
	m_dwUndoView->setWidget( m_undoView );
	m_dwUndoView->setAllowedAreas( Qt::AllDockWidgetAreas );
	addDockWidget( Qt::BottomDockWidgetArea, m_dwUndoView );

	m_tw = new QTabWidget( this );
	setCentralWidget( m_tw );
	loadToolBars();
	
	QSettings s;
	s.beginGroup( "Main_window" );
	restoreGeometry( s.value( "Geometry", saveGeometry() ).toByteArray() );
	restoreState( s.value( "State", saveState() ).toByteArray() );
	s.endGroup();

	QString iSize = s.value( "Options/iconSize" ).toString();
	int h, w;
	h = iSize.section( "x", 0, 0 ).toInt();
	w = iSize.section( "x", 1, 1 ).toInt();
	if ( h == 0 || w == 0 )
		toolBar->setIconSize( QSize( 16, 16 ) );
	else
		toolBar->setIconSize( QSize( w, h ) );

	actionRemove_page->setEnabled( false );

	connect( actionNew_report, SIGNAL( triggered( bool ) ), SLOT( newReport() ) );

	connect( actionSave_report, SIGNAL( triggered( bool ) ), SLOT( saveReport() ) );
	connect( actionSave_report_as, SIGNAL( triggered( bool ) ), SLOT( saveReportAs() ) );

	connect( actionOpen_report, SIGNAL( triggered( bool ) ), SLOT( openReport() ) );
	connect( menuOpen_last_report, SIGNAL( aboutToShow() ), SLOT( prepareLastReportMenu() ) );

	connect( actionOpen_template, SIGNAL( triggered( bool ) ), SLOT( openTemplate() ) );
	connect( menuOpen_last_temlate, SIGNAL( aboutToShow() ), SLOT( prepareLastTemplateMenu() ) );

	connect( actionNew_page, SIGNAL( triggered( bool ) ), SLOT( newPage() ) );
	connect( actionRemove_page, SIGNAL( triggered( bool ) ), SLOT( removePage() ) );

	connect( actionExit, SIGNAL( triggered( bool ) ), SLOT( close() ) );

	connect( actionZoom_in, SIGNAL( triggered( bool ) ), SLOT( zoomIn() ) );
	connect( actionZoom_out, SIGNAL( triggered( bool ) ), SLOT( zoomOut() ) );
	connect( actionZoom_original, SIGNAL( triggered( bool ) ), SLOT( zoomOriginal() ) );
	connect( actionZoom_WYSIWYG, SIGNAL( triggered( bool ) ), SLOT( zoomWYSIWYG() ) );


	connect( actionEdit_script, SIGNAL( triggered( bool ) ), SLOT( editScript() ) );

	connect( actionExecute, SIGNAL( triggered( bool ) ), SLOT( saveReport() ) );
	connect( actionExecute, SIGNAL( triggered( bool ) ), SLOT( executeReport() ) );

	connect( actionSetup_a_database, SIGNAL( triggered( bool ) ), SLOT( setupDatabase() ) );

	connect( actionCopy, SIGNAL( triggered( bool ) ), SLOT( copy() ) );
	connect( actionCut, SIGNAL( triggered( bool ) ), SLOT( cut() ) );
	connect( actionPaste, SIGNAL( triggered( bool ) ), SLOT( paste() ) );
	connect( actionDelete, SIGNAL( triggered( bool ) ), SLOT( del() ) );
	connect( actionSave_item_as, SIGNAL( triggered( bool ) ), SLOT( saveItem() ) );
	connect( actionOpen_item_from, SIGNAL( triggered( bool ) ), SLOT( openItem() ) );

	connect( action_About_eXaro, SIGNAL( triggered( bool ) ), SLOT( about() ) );
	connect( actionOptions, SIGNAL( triggered( bool ) ), SLOT( options() ) );

	connect( actionUndo, SIGNAL( triggered( bool ) ), m_undoStack, SLOT( undo() ) );
	connect( actionRedo, SIGNAL( triggered( bool ) ), m_undoStack, SLOT( redo() ) );

	connect( m_tw, SIGNAL( currentChanged( int ) ), SLOT( currentChanged( int ) ) );

	connect( this, SIGNAL( setCurrentIndex( const QModelIndex&, QItemSelectionModel::SelectionFlags ) ), selectionModel, SLOT( setCurrentIndex( const QModelIndex&, QItemSelectionModel::SelectionFlags ) ) );
	connect( selectionModel, SIGNAL( currentChanged( const QModelIndex & , const QModelIndex & ) ), SLOT( objectChanged( const QModelIndex & , const QModelIndex & ) ) );

	connect( m_pe, SIGNAL( propertyChanged( QObject *, const QString & , const QVariant & , const QVariant & ) ),
	                this, SLOT( propertyChanged( QObject *, const QString & , const QVariant & , const QVariant & ) ) );

	m_contextMenu.addAction( actionCopy );
	m_contextMenu.addAction( actionCut );
	m_contextMenu.addAction( actionPaste );
	m_contextMenu.addAction( actionDelete );
	m_contextMenu.addSeparator();
	m_contextMenu.addAction( actionZoom_in );
	m_contextMenu.addAction( actionZoom_out );
	m_contextMenu.addAction( actionZoom_original );
	m_contextMenu.addSeparator();
	m_contextMenu.addAction( actionSave_item_as );
	m_contextMenu.addAction( actionOpen_item_from );

	m_report = m_reportEngine.reports()[0]->createInstance( 0 );
	m_report->setObjectName( "report" );
	m_report->setName( tr( "report name" ) );
	m_report->setAuthor( "(c) 2008 BogDan" );
	m_pe->setObject( m_report );
	m_nameValidator->setRootObject( m_report );
	m_saveFile = "";

	for ( int i = 0;i < m_reportEngine.items().uniqueKeys().size();i++ )
	{
		m_tb->addItem( new QListWidget( this ), m_reportEngine.items().uniqueKeys()[i] );
		m_tb->setCurrentIndex( m_tb->count() - 1 );

		for ( int j = 0;j < m_reportEngine.items().values( m_reportEngine.items().uniqueKeys()[i] ).count();j++ )
			new QListWidgetItem( m_reportEngine.items().values( m_reportEngine.items().uniqueKeys()[i] )[j]->toolBoxIcon(), m_reportEngine.items().values( m_reportEngine.items().uniqueKeys()[i] )[j]->toolBoxText(), dynamic_cast<QListWidget*>( m_tb->currentWidget() ) );

		dynamic_cast<QListWidget*>( m_tb->currentWidget() )->setCurrentRow( -1 );
	}

	m_tb->setCurrentIndex( 0 );
	m_lastSelectedObject = 0;
	if ( 2 == qApp->arguments().size() )
		openReport( qApp->arguments()[1] );

	m_dwToolBox->toggleViewAction()->setIcon( QIcon( ":/images/button_tool.png" ) );
	m_dwPropertyEditor->toggleViewAction()->setIcon( QIcon( ":/images/button_property.png" ) );
	m_dwQueryEditor->toggleViewAction()->setIcon( QIcon( ":/images/button_sql.png" ) );
	m_dwUiEditor->toggleViewAction()->setIcon( QIcon( ":/images/button_uieditor.png" ) );
	m_dwObjectInspector->toggleViewAction()->setIcon( QIcon( ":/images/button_objects.png" ) );
	m_dwUndoView->toggleViewAction()->setIcon( QIcon( ":/images/button_commands.png" ) );

	menuToolboxes->addAction( m_dwToolBox->toggleViewAction() );
	menuToolboxes->addAction( m_dwPropertyEditor->toggleViewAction() );
	menuToolboxes->addAction( m_dwQueryEditor->toggleViewAction() );
	menuToolboxes->addAction( m_dwUiEditor->toggleViewAction() );
	menuToolboxes->addAction( m_dwObjectInspector->toggleViewAction() );
	menuToolboxes->addAction( m_dwUndoView->toggleViewAction() );

	toolBarTools->addAction( m_dwToolBox->toggleViewAction() );
	toolBarTools->addAction( m_dwPropertyEditor->toggleViewAction() );
	toolBarTools->addAction( m_dwQueryEditor->toggleViewAction() );
	toolBarTools->addAction( m_dwUiEditor->toggleViewAction() );
	toolBarTools->addAction( m_dwObjectInspector->toggleViewAction() );
	toolBarTools->addAction( m_dwUndoView->toggleViewAction() );

	menuToolbars->addAction(toolBar->toggleViewAction());
	menuToolbars->addAction(toolBarTools->toggleViewAction());

	m_objectModel.setRootObject( m_report );

	m_smTemplate = 0;
	m_smReport = 0;
	setupActions();
}

void mainWindow::setupActions()
{
	actionOpen_report->setShortcuts( QKeySequence::Open );
	actionSave_report->setShortcuts( QKeySequence::Save );
	actionNew_page->setShortcuts( QKeySequence::New );
	actionRemove_page->setShortcuts( QKeySequence::Close );
	actionDelete->setShortcuts( QKeySequence::Delete );

	actionCut->setShortcuts( QKeySequence::Cut );
	actionCopy->setShortcuts( QKeySequence::Copy );
	actionPaste->setShortcuts( QKeySequence::Paste );
	actionUndo->setShortcuts( QKeySequence::Undo );
	actionRedo->setShortcuts( QKeySequence::Redo );
	actionZoom_in->setShortcuts( QKeySequence::ZoomIn );
	actionZoom_out->setShortcuts( QKeySequence::ZoomOut );
	connect( m_undoStack, SIGNAL( canUndoChanged( bool ) ), actionUndo, SLOT( setEnabled( bool ) ) );
	connect( m_undoStack, SIGNAL( canRedoChanged( bool ) ), actionRedo, SLOT( setEnabled( bool ) ) );
	actionUndo->setEnabled( m_undoStack->canUndo() );
	actionRedo->setEnabled( m_undoStack->canRedo() );
}

void mainWindow::saveItem()
{
	if ( !m_lastSelectedObject )
		return;
	QString fileName = QFileDialog::getSaveFileName( this, tr( "Save item" ),
	                QDir::homePath() + "/item.bdrti", tr( "Item (*.bdrti)" ) );
	if ( !fileName.length() )
		return;

	QFile file( fileName );

	if ( file.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
		QDomDocument doc( "report" );
		doc.appendChild( Report::ReportEngine::objectProperties( m_lastSelectedObject, &doc ) );
		file.write( doc.toByteArray( 4 ) );
		file.close();
	}
}

void mainWindow::openItem()
{
	m_lastSelectedObject = m_pe->object();
	if ( !dynamic_cast<Report::ItemInterface*>( m_lastSelectedObject ) && !dynamic_cast<Report::PageInterface *>( m_lastSelectedObject ) )
		return;

	QString reportName = QFileDialog::getOpenFileName( this, tr( "Open report" ),
	                QDir::homePath() + "", tr( "Item (*.bdrti)" ) );

	if ( !reportName.size() )
		return;

	QFile file( reportName );
	if ( file.open( QIODevice::ReadOnly ) )
	{
		if ( dynamic_cast<Report::ItemInterface*>( m_lastSelectedObject ) )
			m_undoStack->push( new AddDomObject( dynamic_cast<Report::PageInterface *>( dynamic_cast<Report::ItemInterface*>( m_lastSelectedObject )->scene() ), m_lastSelectedObject->objectName(), file.readAll(),  m_lastSelectedObjectPos, this ) );
		else
			m_undoStack->push( new AddDomObject( dynamic_cast<Report::PageInterface *>( m_lastSelectedObject ), m_lastSelectedObject->objectName(), file.readAll(),  m_lastSelectedObjectPos, this ) );
	}
}

void mainWindow::about()
{
	AboutDialog d;
	d.exec();
}

void mainWindow::objectChanged( const QModelIndex & current, const QModelIndex & /*previous */ )
{
	m_pe->setObject( reinterpret_cast<ObjectModel::ObjectStruct*>( current.internalPointer() )->object );
	foreach( QObject * obj, m_report->children() )
		if ( dynamic_cast<Report::PageInterface *>( obj ) )
			dynamic_cast<Report::PageInterface *>( obj )->clearSelection();

	if ( dynamic_cast<Report::ItemInterface *>( reinterpret_cast<ObjectModel::ObjectStruct*>( current.internalPointer() )->object ) )
		dynamic_cast<Report::ItemInterface *>( reinterpret_cast<ObjectModel::ObjectStruct*>( current.internalPointer() )->object )->setSelected( true );
}


bool mainWindow::selectObject( QObject * object, QModelIndex index )
{
	if ( index.isValid() && reinterpret_cast<ObjectModel::ObjectStruct *>( index.internalPointer() )->object == object )
	{
		emit( setCurrentIndex( index, QItemSelectionModel::SelectCurrent ) );
		return true;
	}

	for ( int i = 0;i < m_objectModel.rowCount( index );i++ )
		if ( selectObject( object, m_objectModel.index( i, 0, index ) ) )
			return true;
	return false;
}

bool mainWindow::askToSaveReport()
{
	if ( m_undoStack->index() != m_lastUndoIndex )
	{
		switch(QMessageBox::question( this, tr( "eXaro" ), tr( "Save changes ?" ), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel ) )
		{
			case QMessageBox::Yes:
				saveReport();
				if ( !m_saveFile.length() )
					return false;
				break;

			case QMessageBox::Cancel:
				return false;

			default:
				return true;
		}
	}
	return true;
}

void mainWindow::setMagnetActions( Report::PageInterface* page )
{
	if ( !page )
		return;

	connect( actionLeft_magnet, SIGNAL( toggled( bool ) ), page, SLOT( setLeftMagnet( bool ) ) );
	connect( actionRight_magnet, SIGNAL( toggled( bool ) ), page, SLOT( setRightMagnet( bool ) ) );
	connect( actionTop_magnet, SIGNAL( toggled( bool ) ), page, SLOT( setTopMagnet( bool ) ) );
	connect( actionBottom_magnet, SIGNAL( toggled( bool ) ), page, SLOT( setBottomMagnet( bool ) ) );
	connect( actionVertical_magnet, SIGNAL( toggled( bool ) ), page, SLOT( setVerticalMagnet( bool ) ) );
	connect( actionHorizontal_magnet, SIGNAL( toggled( bool ) ), page, SLOT( setHorizontalMagnet( bool ) ) );

	page->setLeftMagnet( actionLeft_magnet->isChecked() );
	page->setRightMagnet( actionRight_magnet->isChecked() );
	page->setTopMagnet( actionTop_magnet->isChecked() );
	page->setBottomMagnet( actionBottom_magnet->isChecked() );
	page->setVerticalMagnet( actionVertical_magnet->isChecked() );
	page->setHorizontalMagnet( actionHorizontal_magnet->isChecked() );
}

void mainWindow::copy()
{
	if ( m_lastSelectedObject )
		m_reportEngine.copy( m_lastSelectedObject );
}

void mainWindow::pasteItem( QObject * item )
{
	connect( item, SIGNAL( itemSelected( QObject *, QPointF ) ), SLOT( itemSelected( QObject *, QPointF ) ) );
	connect( item, SIGNAL( geometryChanged( QObject*, QRectF, QRectF ) ), SLOT( itemGeometryChanged( QObject*, QRectF, QRectF ) ) );
	item->setObjectName( Report::ReportEngine::uniqueName( item->metaObject()->className(), m_report ) );
	foreach( QObject * obj, item->children() )
		pasteItem( obj );
}

void mainWindow::paste()
{
	if ( !m_lastSelectedObject )
		return;

	QObject * obj = m_reportEngine.paste( m_lastSelectedObject );
	Report::ItemInterface* item = dynamic_cast<Report::ItemInterface*>( obj );
	if ( dynamic_cast<Report::ItemInterface*>( m_lastSelectedObject ) )
	{
		if ( !dynamic_cast<Report::ItemInterface*>( m_lastSelectedObject )->canContain( item ) )
		{
			delete item;
			item = 0;
		}
	}
	else
		if ( dynamic_cast<Report::PageInterface*>( m_lastSelectedObject )->canContain( item ) )
			dynamic_cast<Report::PageInterface*>( m_lastSelectedObject )->addItem( item );
		else
		{
			delete item;
			item = 0;
		}

	if ( item )
	{
		pasteItem( item );
		if ( dynamic_cast<Report::BandInterface*>( item ) )
			dynamic_cast<Report::BandInterface*>( item )->setOrder( INT_MAX );

		m_pe->setObject( item );
		m_objectModel.setRootObject( m_report );
		selectObject( item, m_objectModel.index( 0, 0 ) );
	}
}

void mainWindow::del()
{
	QGraphicsItem* item;
	if ( !( item = dynamic_cast<QGraphicsItem*>( m_lastSelectedObject ) ) || !dynamic_cast<Report::ItemInterface*>( m_lastSelectedObject ) )
		return;
	QUndoCommand *delCommand = new DelCommand( dynamic_cast<Report::ItemInterface*>( item ), this );
	m_undoStack->push( delCommand );
}

void mainWindow::cut()
{
	copy();
	del();
}

mainWindow::~mainWindow()
{
}


void mainWindow::setupDatabase()
{
	SqlDatabaseDialog d;
	d.exec();
}

void mainWindow::executeReport()
{
	m_report->exec();
}

void mainWindow::closeEvent( QCloseEvent *event )
{
	QSettings s;
	s.beginGroup( "Main_window" );
	s.setValue( "Geometry", saveGeometry() );
	s.setValue( "State", saveState() );
	s.endGroup();
	if ( askToSaveReport() )
		event->accept();
	else
		event->ignore();
}

void mainWindow::newReport()
{
	if ( !askToSaveReport() )
		return;
	while ( m_tw->count() )
	{
		m_tw->setCurrentIndex( 0 );
		removePage();
	}
	delete m_report;
	m_pe->setObject(0);
	m_objectModel.setRootObject(0);
	m_nameValidator->setRootObject( 0 );

	reportWizard rw(&m_reportEngine);
	if (QDialog::Accepted==rw.exec())
	{
		m_report=rw.report();
		setupReport();
	}
	else
	{
		m_report = m_reportEngine.reports()[0]->createInstance( 0 );
		m_report->setObjectName( "report" );
		m_report->setName( tr( "Report name" ) );
		m_report->setAuthor( "(c) 2008 BogDan" );
		m_pe->setObject( m_report );
		m_saveFile = "";
		m_undoStack->clear();
	}
	m_lastUndoIndex=-1;
	setWindowTitle( tr( "eXaro v%1 unsaved report" ).arg( EXARO_VERSION ) );
}

void mainWindow::editScript()
{
	ScriptEditDialog se;
	se.setText( m_report->script() );
	if ( QDialog::Accepted == se.exec() )
		m_report->setScript( se.text() );
}

void mainWindow::connectItem( QObject * obj )
{
	connect( obj, SIGNAL( itemSelected( QObject *, QPointF ) ), this, SLOT( itemSelected( QObject *, QPointF ) ) );
	connect( obj, SIGNAL( geometryChanged( QObject*, QRectF, QRectF ) ), this, SLOT( itemGeometryChanged( QObject*, QRectF, QRectF ) ) );
	foreach( QObject * child, obj->children() )
		connectItem( child );
}

void mainWindow::setupReport() 
{
	QGraphicsView * gw = 0;

	for ( int p = 0; p < m_report->children().size();p++ )
	{
		if ( !dynamic_cast<QGraphicsScene*>( m_report->children()[p] ) )
			continue;

		dynamic_cast<Report::PageInterface*>( m_report->children()[p] )->setContextMenu( &m_contextMenu );
		gw = new QGraphicsView( dynamic_cast<QGraphicsScene*>( m_report->children()[p] ) );
		m_tw->addTab(( QWidget* ) gw, dynamic_cast<Report::PageInterface*>( gw->scene() )->objectName() );
		dynamic_cast<QGraphicsScene*>( m_report->children()[p] )->update();
		connect( m_report->children()[p], SIGNAL( itemSelected( QObject *, QPointF ) ), this, SLOT( itemSelected( QObject *, QPointF ) ) );
	        connect(m_report->children()[p], SIGNAL(itemMoved(QObject*, QPointF)), this, SLOT (itemMoved(QObject*, QPointF)) );
		foreach (QObject * obj, m_report->children()[p]->children())
			connectItem( obj );
		setMagnetActions( dynamic_cast<Report::PageInterface*>( gw->scene() ) );
		gw->centerOn( 0, 0 );
	}

	actionRemove_page->setEnabled( m_tw->count() );

	m_dquery->setQueries( m_report->queries() );
	m_dui->setUis( m_report->uis() );
	m_pe->setObject( m_report );
	m_nameValidator->setRootObject( m_report );
	m_objectModel.setRootObject( m_report );
	zoomWYSIWYG();
	m_undoStack->clear();
	m_lastUndoIndex=m_undoStack->index();
}

void mainWindow::openReport( const QString & report )
{
	if ( !askToSaveReport() )
		return;

	QSettings s;
	QStringList list = s.value( "Designer/lastReports" ).toString().split( ";;", QString::SkipEmptyParts );
	if ( !list.contains( report ) )
		list.append( report );
	if ( list.count() > ROWS_IN_MENU )
		list.removeFirst();

	QFileInfo f( report );
	s.setValue( "Designer/reportDir", f.absolutePath() );
	s.setValue( "Designer/lastReports", list.join( ";;" ) );


	QFile file( report );
	if ( file.open( QIODevice::ReadOnly ) )
	{
		m_pe->setObject( 0 );
		while ( m_tw->count() )
		{
			m_tw->setCurrentIndex( 0 );
			removePage();
		}
		delete m_report;
		m_report = dynamic_cast<Report::ReportInterface*>( m_reportEngine.loadReport( &file ) );
		file.close();
	}

	if ( !m_report )
	{
		newReport();
		return;
	}

	setupReport();

	m_saveFile = report;
	setWindowTitle( tr( "eXaro v%1 (%2)" ).arg( EXARO_VERSION ).arg(report) );
}

void mainWindow::openReport()
{
	QSettings s;
	QString reportDir = s.value( "Designer/reportDir" ).toString();
	if ( reportDir.isEmpty() )
		reportDir = QDir::homePath();

	QString fileName = QFileDialog::getOpenFileName( this, tr( "Open report" ),
	                reportDir , tr( "Report (*.bdrt)" ) );
	if ( !fileName.size() )
		return;

	openReport( fileName );
}

void mainWindow::openTemplate()
{
	QSettings s;
	QString templateDir = s.value( "Designer/templeteDir" ).toString();
	if ( templateDir.isEmpty() )
		templateDir = QDir::homePath();

	QString report = QFileDialog::getOpenFileName( this, tr( "Open template" ),
	                templateDir , tr( "Exaro Template (*.extt)" ) );

	if ( report.isEmpty() )
		return;

	openTemplate( report );
}

void mainWindow::prepareLastReportMenu()
{
	menuOpen_last_report->clear();
	if ( m_smReport )
		delete m_smReport;
	m_smReport = new QSignalMapper( this );

	QSettings s;
	QStringList list = s.value( "Designer/lastReports" ).toString().split( ";;", QString::SkipEmptyParts );

	for ( int i = 0; i < list.count(); ++i )
	{
		QAction *action = new QAction( list.at( i ), this );
		action->setData( list.at( i ) );
		menuOpen_last_report->addAction( action );
		connect( action, SIGNAL( triggered() ), m_smReport , SLOT( map() ) );
		m_smReport ->setMapping( action, list.at( i ) );
	}

	connect( m_smReport, SIGNAL( mapped( const QString & ) ), this, SLOT( openReport( const QString & ) ) );
}

void mainWindow::prepareLastTemplateMenu()
{
	menuOpen_last_temlate->clear();
	if ( m_smTemplate )
		delete m_smTemplate;
	m_smTemplate = new QSignalMapper( this );

	QSettings s;
	QStringList list = s.value( "Designer/lastTemplates" ).toString().split( ";;", QString::SkipEmptyParts );

	for ( int i = 0; i < list.count(); ++i )
	{
		QAction *action = new QAction( list.at( i ), this );
		action->setData( list.at( i ) );
		menuOpen_last_temlate->addAction( action );
		connect( action, SIGNAL( triggered() ), m_smTemplate, SLOT( map() ) );
		m_smTemplate->setMapping( action, list.at( i ) );
	}

	connect( m_smTemplate, SIGNAL( mapped( const QString & ) ), SLOT( openTemplate( const QString & ) ) );

}

void mainWindow::openTemplate( const QString & fileName )
{
	if ( !askToSaveReport() )
		return;

	QSettings s;
	QStringList list = s.value( "Designer/lastTemplates" ).toString().split( ";;", QString::SkipEmptyParts );
	if ( !list.contains( fileName ) )
		list.append( fileName );
	if ( list.count() > ROWS_IN_MENU )
		list.removeFirst();
	QFileInfo f( fileName );
	s.setValue( "Designer/templeteDir", f.absolutePath() );
	s.setValue( "Designer/lastTemplates", list.join( ";;" ) );
	openReport( fileName );
	m_saveFile = "";
}

void mainWindow::options()
{
	OptionsDialog d( this );
	d.exec();
        QSettings s;
        QString iSize = s.value( "Options/iconSize" ).toString();
        int h, w;
        h = iSize.section( "x", 0, 0 ).toInt();
        w = iSize.section( "x", 1, 1 ).toInt();
        if ( h == 0 || w == 0 )
                toolBar->setIconSize( QSize( 16, 16 ) );
        else
                toolBar->setIconSize( QSize( w, h ) );
}


void mainWindow::saveReport()
{
	if ( !m_saveFile.length() )
		m_saveFile = QFileDialog::getSaveFileName( this, tr( "Save report" ),
		                QDir::homePath() + "/newReport.bdrt", tr( "Report (*.bdrt);;Template (*.extt)" ) );

	if ( !m_saveFile.length() )
		return;

	m_report->setQueries( m_dquery->queries() );
	m_report->setUis( m_dui->uis() );

	if ( !m_reportEngine.saveReport( m_saveFile, m_report ) )
		throw( QString( "Can't save the report" ) );
        m_lastUndoIndex=m_undoStack->index();
        setWindowTitle( tr( "eXaro v%1 (%2)" ).arg( EXARO_VERSION ).arg(m_saveFile) );
}

void mainWindow::saveReportAs()
{
	m_saveFile = "";
	saveReport();
}

void mainWindow::zoomIn()
{
	if ( dynamic_cast<QGraphicsView*>( m_tw->currentWidget() ) )
		dynamic_cast<QGraphicsView*>( m_tw->currentWidget() )->scale( 1.1, 1.1 );
}

void mainWindow::zoomOut()
{
	if ( dynamic_cast<QGraphicsView*>( m_tw->currentWidget() ) )
		dynamic_cast<QGraphicsView*>( m_tw->currentWidget() )->scale( 0.9, 0.9 );
}

void mainWindow::zoomWYSIWYG()
{
	if ( dynamic_cast<QGraphicsView*>( m_tw->currentWidget() ) )
	{
		dynamic_cast<QGraphicsView*>( m_tw->currentWidget() )->resetMatrix();
		dynamic_cast<QGraphicsView*>( m_tw->currentWidget() )->scale(( double )QDesktopWidget().screen()->width() / ( screen_widthMM*10 ), ( double )QDesktopWidget().screen()->height() / ( screen_heightMM*10 ) );
		dynamic_cast<QGraphicsView*>( m_tw->currentWidget() )->centerOn( 0, 0 );
	}
}

void mainWindow::zoomOriginal()
{
	if ( dynamic_cast<QGraphicsView*>( m_tw->currentWidget() ) )
	{
		dynamic_cast<QGraphicsView*>( m_tw->currentWidget() )->resetMatrix();
		dynamic_cast<QGraphicsView*>( m_tw->currentWidget() )->centerOn( 0, 0 );
	}
}

void mainWindow::keyReleaseEvent( QKeyEvent * event )
{
	QListWidget * lw = dynamic_cast<QListWidget*>( m_tb->currentWidget() );

	if ( lw )
		lw-> setCurrentRow( -1 );

	QMainWindow::keyReleaseEvent( event );
}

void mainWindow::newPage()
{
	if ( !m_reportEngine.pages().count() )
		return;
	QUndoCommand *newPageCommand = new NewPageCommand( this );
	m_undoStack->push( newPageCommand );
}

void mainWindow::selectLastObject()
{
	itemSelected(m_lastSelectedObject ,QPointF(0,0));
}

void mainWindow::itemSelected( QObject *object, QPointF pos )
{
	m_lastSelectedObject = object;
	m_lastSelectedObjectPos = pos;
	QListWidget* lw = dynamic_cast<QListWidget*>( m_tb->currentWidget() );

	if ( lw && lw->currentRow() > -1 )
	{
		const char* needClassName = ( m_reportEngine.items().values( m_reportEngine.items().uniqueKeys()[m_tb->currentIndex()] )[lw->currentRow()] )->metaObject()->className();
		QPointF absPos = dynamic_cast<Report::PageInterface*>( object ) ? pos : dynamic_cast<Report::ItemInterface*>( object )->mapToScene( pos );
		Report::PageInterface* page = dynamic_cast<Report::PageInterface*>( object ) ? dynamic_cast<Report::PageInterface*>( object )
		                : dynamic_cast<Report::PageInterface*>( dynamic_cast<Report::ItemInterface*>( object )->scene() );

		QUndoCommand *addCommand = new AddCommand( page, needClassName, absPos, this );
		m_undoStack->push( addCommand );
		lw->setCurrentRow( -1 );
	}
	else
	{
		m_pe->setObject( object );
		selectObject( object, m_objectModel.index( 0, 0 ) );
	}
}

void mainWindow::removePage()
{
	QUndoCommand *removePageCommand = new RemovePageCommand( this, m_tw->currentIndex() );
	m_undoStack->push( removePageCommand );
}

void mainWindow::currentChanged( int index )
{
	if ( index < 0 )
	{
		m_pe->setObject( 0 );
		return;
	}
	m_pe->setObject( dynamic_cast<Report::PageInterface*>( dynamic_cast<QGraphicsView*>( m_tw->widget( index ) )->scene() ) );
}

void mainWindow::itemMoved( QObject *movedItem, QPointF movedFromPosition )
{
	QUndoCommand *moveCommand = new MoveCommand( dynamic_cast<Report::ItemInterface*>( movedItem ), movedFromPosition, this );
	m_undoStack->push( moveCommand );
}

void mainWindow::propertyChanged( QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value )
{
	QUndoCommand *propertyChangeCommand = new PropertyChangeCommand( obj, propertyName, old_value, new_value, this );
	m_undoStack->push( propertyChangeCommand );
}

void mainWindow::itemGeometryChanged( QObject* object, QRectF newGeometry, QRectF oldGeometry )
{
	QUndoCommand *geometryChangeCommand = new GeometryChangeCommand( object, newGeometry, oldGeometry, this );
	m_undoStack->push( geometryChangeCommand );
}


int mainWindow::_createNewPage_(Report::PageInterface* page,int afterIndex, QString pageName )
{
	QGraphicsView * gw = 0;

	if ( 1 == m_reportEngine.pages().count() ) {
	    if (!page)
		gw = new QGraphicsView( static_cast<QGraphicsScene*>( m_reportEngine.pages()[0]->createInstance( m_report ) ) );
	    else
		gw = new QGraphicsView( static_cast<QGraphicsScene*>(page) );
	    }
	else
	{
#ifndef WIN32
#warning implement me !!!!
#endif
		//ehe tata ici e de lucru
	}

	/*
			gw->setAutoFillBackground(true);
			gw->setBackgroundRole(QPalette::Base);
			QPalette pal = gw->palette();
			pal.setBrush(QPalette::Base, Qt::blue);
			pal.setColor(QPalette::HighlightedText, Qt::red);
			gw->setPalette(pal);
			gw->scene()->setBackgroundBrush(QPixmap(":/images/background.png"));
	*/
	QString nameOfPage = pageName.isNull() ? Report::ReportEngine::uniqueName( dynamic_cast<Report::PageInterface*>( gw->scene() )->metaObject()->className() , m_report ) : pageName;

	dynamic_cast<Report::PageInterface*>( gw->scene() )->setObjectName( nameOfPage );
	dynamic_cast<Report::PageInterface*>( gw->scene() )->setContextMenu( &m_contextMenu );


	int m_index = m_tw->insertTab( afterIndex, ( QWidget* ) gw, dynamic_cast<Report::PageInterface*>( gw->scene() )->objectName() );
	m_tw->setCurrentWidget(( QWidget* ) gw );

	actionRemove_page->setEnabled( m_tw->count() );

	connect( dynamic_cast<Report::PageInterface*>( gw->scene() ), SIGNAL( itemSelected( QObject *, QPointF ) ), this, SLOT( itemSelected( QObject *, QPointF ) ) );
	connect( dynamic_cast<Report::PageInterface*>( gw->scene() ), SIGNAL( itemMoved( QObject*, QPointF ) ), this, SLOT( itemMoved( QObject*, QPointF ) ) );

	setMagnetActions( dynamic_cast<Report::PageInterface*>( gw->scene() ) );

	if ( 1 == m_tw->count() )
		m_pe->setObject( dynamic_cast<Report::PageInterface*>( gw->scene() ) );

	m_objectModel.setRootObject( m_report );
	zoomWYSIWYG();
	return m_index;
}


void mainWindow::_deletePage_( int index )
{
	if ( dynamic_cast<QGraphicsView*>( m_tw->widget( index ) ) )
		delete dynamic_cast<QGraphicsView*>( m_tw->widget( index ) )->scene();

	m_tw->removeTab( index );
	actionRemove_page->setEnabled( m_tw->count() );

	if ( !m_tw->count() )
		m_pe->setObject( m_report );
	m_objectModel.setRootObject( m_report );
}

void mainWindow::loadToolBars()
{
	QDir pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
	if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
		pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS")
	{
		pluginsDir.cdUp();
		pluginsDir.cd("PlugIns");
	}
#else
	pluginsDir.cd(LIB_INSTALL_DIR);
#endif
	pluginsDir.cd("exaro");
	pluginsDir.cd("toolbars");
	QStringList paths=qApp->libraryPaths();
	paths<<pluginsDir.absolutePath();
	qApp->setLibraryPaths(paths);
	foreach(QString fileName, pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader loader;
		loader.setLoadHints(QLibrary::ResolveAllSymbolsHint|QLibrary::ExportExternalSymbolsHint);
		loader.setFileName(pluginsDir.absoluteFilePath(fileName));
		if (!loader.load())
		{
			qCritical() << loader.errorString();
			continue;
		}
		QObject *plugin = loader.instance();
		QToolBar* tbp=dynamic_cast<QToolBar*>(plugin);
		if (tbp)
		{
			addToolBar(tbp);
			connect(m_pe, SIGNAL(objectChanged(QObject*)), tbp, SLOT(setObject(QObject*)));
			connect(m_pe, SIGNAL(propertyChanged(QObject*,QString,QVariant,QVariant)), tbp, SLOT(propertyChanged(QObject*,QString,QVariant,QVariant)));
			connect(tbp, SIGNAL(propertyChanged()), m_pe, SLOT(resetProperties()));
			menuToolbars->addAction(tbp->toggleViewAction());
		}
		else
			delete plugin;
	}
}

