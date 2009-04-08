#ifndef DATASETEDITOR_H
#define DATASETEDITOR_H

#include <QWidget>

class DataSet;

class DataSetEditor : public QWidget
{
public:
    DataSetEditor(QWidget * parent = 0);

    virtual void setDataset(DataSet* dtst);
    virtual DataSet * dataset();
    virtual void sync();

private:
    DataSet * m_dataset;
};

#endif // DATASETEDITOR_H
