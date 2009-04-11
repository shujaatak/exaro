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

#ifndef CSVDATASETEDITOR_H
#define CSVDATASETEDITOR_H

#include <dataseteditor.h>
#include "dataset.h"

namespace Ui {
    class CsvDatasetEditor;
}

class CsvDatasetEditor : public DataSetEditor
{
    Q_OBJECT
    Q_DISABLE_COPY(CsvDatasetEditor)
public:
    explicit CsvDatasetEditor(QWidget *parent = 0);
    virtual ~CsvDatasetEditor();

    void setDataset(Report::DataSet* dtst);
    void sync();
protected:
    virtual void changeEvent(QEvent *e);
private slots:
    void on_bBrowse_clicked();
    void on_bInject_clicked();

private:
    Ui::CsvDatasetEditor *m_ui;
};

#endif // CSVDATASETEDITOR_H
