#ifndef SQLDATASETEDITOR_H
#define SQLDATASETEDITOR_H

#include <dataseteditor.h>
#include "dataset.h"

namespace Ui {
    class SqlDatasetEditor;
}

class SqlDatasetEditor : public DataSetEditor
{
    Q_OBJECT
    Q_DISABLE_COPY(SqlDatasetEditor)
public:
    explicit SqlDatasetEditor(QWidget *parent = 0);
    virtual ~SqlDatasetEditor();

    void setDataset(Report::DataSet* dtst);
    void sync();
protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SqlDatasetEditor *m_ui;
};

#endif // SQLDATASETEDITOR_H
