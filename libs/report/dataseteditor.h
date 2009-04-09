#ifndef DATASETEDITOR_H
#define DATASETEDITOR_H

#include <QWidget>
#include <dataset.h>

class DataSetEditor : public QWidget
{
public:
    DataSetEditor(QWidget * parent = 0);

    virtual void setDataset(Report::DataSet* dtst);
    virtual Report::DataSet * dataset();
    virtual void sync();

protected:
    Report::DataSet * m_dataset;
};

#endif // DATASETEDITOR_H
