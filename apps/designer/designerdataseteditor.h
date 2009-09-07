/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by Mikhalov Alexander                              *
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


#ifndef DESIGNERDATASETEDITOR_H
#define DESIGNERDATASETEDITOR_H

#include <QWidget>
#include <QHash>
#include <QtSql/QSqlTableModel>
#include "ui_designerdataseteditor.h"

class DataSetEditor;

namespace Report
{

class ReportInterface;
class ReportEngine;

class DesignerDatasetEditor : public QWidget, public Ui::DesignerDatasetEditor
{
	Q_OBJECT

private:
	QSqlTableModel*	m_dataTableModel;
	ReportInterface * m_report;
	ReportEngine * m_engine;
	QHash <QString, DataSetEditor*> m_editors;
	DataSetEditor * currentEditor;
	QStringList currentVars;

protected:
	void refreshButtons();

public:
	DesignerDatasetEditor(ReportEngine * engine, QWidget* parent = 0, Qt::WFlags fl = 0);
	~DesignerDatasetEditor();


	void setReport(ReportInterface * report);

//	QMap <QString, QVariant> queries();
//	void setQueries(QMap <QString, QVariant> queries);
//	void refreshVariables();
	QStringList variables();

signals:
//	void removeVar(QString var);
//	void addVar(QString var);
	void refreshVariables();

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

