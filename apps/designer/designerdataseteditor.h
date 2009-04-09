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


#ifndef DESIGNERDATASETEDITOR_H
#define DESIGNERDATASETEDITOR_H

#include <QWidget>
#include <QHash>
#include <QtSql/QSqlTableModel>
#include "ui_designerdataseteditor.h"

class DataSetEditor;

/*
class ListWidgetItem: public QListWidgetItem
{
    public:
    QString className;
};
*/

namespace Report
{

class ReportInterface;
class ReportEngine;

class DesignerDatasetEditor : public QWidget, public Ui::DesignerDatasetEditor
{
	Q_OBJECT

private:
//	QMap <QString, QVariant> m_queries;
//	SQLSyntaxHighlighter m_syntax;
//	QSqlQueryModel 	m_queryModel;
	QSqlTableModel*	m_dataTableModel;
	ReportInterface * m_report;
	ReportEngine * m_engine;
//	QStackedWidget *stackedWidget;
	QHash <QString, DataSetEditor*> m_editors;
	DataSetEditor * currentEditor;

protected:
	void refreshButtons();

public:
	DesignerDatasetEditor(ReportEngine * engine, QWidget* parent = 0, Qt::WFlags fl = 0);
	~DesignerDatasetEditor();


	void setReport(ReportInterface * report);

//	QMap <QString, QVariant> queries();
//	void setQueries(QMap <QString, QVariant> queries);

private slots:
	void on_bDatasetExec_clicked();
	void on_m_listWidget_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
	void on_tablesList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
	void on_b_properties_toggled ( bool checked );

public slots:
	void createItem();
	void deleteItem();
	void editName();
	void resetConnection();
	void fillTablesList();
	void sync();
};

}

#endif

