#include "dataseteditor.h"
#include "dataset.h"

DataSetEditor::DataSetEditor(QWidget * parent)
    : QWidget (parent), m_dataset(0)
{
}

void DataSetEditor::setDataset(DataSet* dtst) {m_dataset = dtst;}
DataSet * DataSetEditor::dataset() {return m_dataset;}
void DataSetEditor::sync() {};
