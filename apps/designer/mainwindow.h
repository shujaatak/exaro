/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bogdan@licentia.eu                                                    *
 *   Copyright (C) 2009 by Mikhalov Alexaner                               *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QDomDocument>
#include <QSignalMapper>
#include "reportengine.h"
#include "ui_mainwindow.h"
#include "propertyeditor.h"
#include "designerdataseteditor.h"
#include "designerscriptwidget.h"
#include "designeruiwidget.h"
#include "objectmodel.h"
#include "command.h"
#include "layoutmanager.h"

class QTabWidget;
class QTreeView;
class NameValidator;
class QUndoStack;
class QUndoView;
class ItemsToolBar;
class PageView;

class mainWindow : public QMainWindow, private Ui::mainWindow
{
	Q_OBJECT

public:
	mainWindow(QWidget* parent = 0, Qt::WFlags fl = 0);
	~mainWindow();

	void closeEvent(QCloseEvent *event);
	Report::ReportEngine * reportEngine();

protected:
	void keyReleaseEvent(QKeyEvent * event);
	void connectItem(QObject * item);
	void pasteItem(QObject * item);
	void setMagnetActions(Report::PageInterface* page);
	bool askToSaveReport();

private:
	bool selectObject(QObject * object, QModelIndex index, QItemSelectionModel::SelectionFlag selFlag = QItemSelectionModel::ClearAndSelect);
	int _createNewPage_(Report::PageInterface* page = 0, int afterIndex = -1, QString pageName = QString());
	void _deletePage_(int index);
	void setupActions();
	void refreshReportBeholders(Report::ReportInterface* report);
	void restoreSettings(bool withState = true);
	void refreshSelectionActions();

private:
	ObjectModel m_objectModel;
	QTreeView * m_objectInspector;
	Report::ReportEngine m_reportEngine;
	QString m_saveFile, m_designerTool;
	QTabWidget *m_tw;
	PropertyEditor::PropertyEditor *m_pe;
	Report::DesignerDatasetEditor * m_wdataset;
	Report::DesignerScriptWidget *m_dscript;
	Report::DesignerUiWidget * m_dui;
	Report::ReportInterface* m_report;
	QMenu	m_contextMenu;
	QDockWidget * m_dwPropertyEditor;
	QDockWidget * m_dwQueryEditor;
	QDockWidget * m_dwUiEditor;
	QDockWidget * m_dwObjectInspector;
	QDockWidget * m_dwUndoView;
	QSignalMapper * m_smTemplate;
	NameValidator * m_nameValidator;
	QSignalMapper * m_smReport;
	QUndoStack *undoStack;
	QUndoView *undoView;
	QToolBar *itemLayoutToolbar;
	QDomDocument lastCopyGroup;
	QLabel messageLabel;
	QLabel messageGeometryLabel;
	QLabel messageMousePosLabel;
	ItemsToolBar * toolBarItems;
	PageView * m_lastView, *m_currentView;

public slots:
	void selectLastObject();

protected slots:
	void newPage();
	void removePage();
	void currentChanged(int);
	void itemSelected(QObject *, QPointF, Qt::KeyboardModifiers /*= Qt::NoModifier*/);
	void zoomIn();
	void zoomOut();
	void zoomOriginal();
//	void zoomWYSIWYG();
	void saveReport();
	void saveReportAs();
	void openReport(bool notAsk = false);
	void openReport(const QString & report, bool notAsk = false);
	void newReport(bool notAsk = false);
	void openTemplate();
	void openTemplate(const QString &);
	void prepareLastTemplateMenu();
	void prepareLastReportMenu();
	void executeReport();
	void setupDatabase();
	void copy();
	void paste();
	void del();
	void cut();
	void objectInspectorObjectChanged(const QModelIndex & current, const QModelIndex & previous);
	void about();
	void options();
	void saveItem();
	void openItem();
	void itemMoved(QObject* movedItem, QPointF movedFromPosition);
	void propertyChanged(QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value);
	void itemGeometryChanged(QObject* object, QRectF newGeometry, QRectF oldGeometry);
	void on_actionBandUp_triggered();
	void on_actionBandDown_triggered();
	void on_actionLastConnect_triggered();
	void objectInspector_pressed(const QModelIndex & index);
	void moveSelectionUp();
	void moveSelectionDown();
	void moveSelectionLeft();
	void moveSelectionRight();
	void showStatusBarItemGeometry(QObject*, QRectF rect);
	void addItem(Report::ItemInterface *, QPointF);
	void on_actionAlign_Selection_Top_triggered();
	void on_actionAlign_Selection_Bottom_triggered();
	void on_actionAlign_Selection_Left_triggered();
	void on_actionAlign_Selection_Right_triggered();
	void on_actionAlign_Selection_Height_triggered();
	void on_actionAlign_Selection_Width_triggered();
signals:
	void setCurrentIndex(const QModelIndex & , QItemSelectionModel::SelectionFlags);

	friend class AddCommand;
	friend class MoveCommand;
	friend class DelCommand;
	friend class PropertyChangeCommand;
	friend class GeometryChangeCommand;
	friend class NewPageCommand;
	friend class RemovePageCommand;
	friend class AddDomObject;

	friend class ItemsToolBar;
//friend class ChangePageCommand;
};

#endif

