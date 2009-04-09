#include "sqldataseteditor.h"
#include "sqldataset.h"
#include "ui_sqldataseteditor.h"
#include "sqlsyntaxhighlighter.h"

SqlDatasetEditor::SqlDatasetEditor(QWidget *parent) :
    DataSetEditor(parent),
    m_ui(new Ui::SqlDatasetEditor)
{
    m_ui->setupUi(this);

    SQLSyntaxHighlighter * syntax = new SQLSyntaxHighlighter(m_ui->textEdit->document());
}

SqlDatasetEditor::~SqlDatasetEditor()
{
    delete m_ui;
}

void SqlDatasetEditor::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SqlDatasetEditor::setDataset(Report::DataSet* dtst)
{
    Q_ASSERT(dtst);
    m_dataset = dtst;
    if (! dynamic_cast<SqlDataset*> (dtst))
	return;

    m_ui->textEdit->setPlainText(dynamic_cast<SqlDataset*> (dtst)->text());
}

void SqlDatasetEditor::sync()
{
    if (!dynamic_cast<SqlDataset*> (m_dataset))
	return;
    dynamic_cast<SqlDataset*> (m_dataset)->setText( m_ui->textEdit->toPlainText());
}

