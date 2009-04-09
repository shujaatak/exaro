#include "dataseteditor.h"
#include "dataset.h"

DataSetEditor::DataSetEditor(QWidget * parent)
    : QWidget (parent), m_dataset(0)
{
}

void DataSetEditor::setDataset(Report::DataSet* dtst) {m_dataset = dtst;}
Report::DataSet * DataSetEditor::dataset() {return m_dataset;}
void DataSetEditor::sync() {};
