#include "newdatasetdialog.h"
#include "ui_newdatasetdialog.h"

NewDatasetDialog::NewDatasetDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::NewDatasetDialog)
{
    m_ui->setupUi(this);
}

NewDatasetDialog::~NewDatasetDialog()
{
    delete m_ui;
}

void NewDatasetDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void NewDatasetDialog::addTypes(QStringList str)
{
    m_ui->cbType->addItems(str);
}

void NewDatasetDialog::addType(QString str)
{
    m_ui->cbType->addItem(str);
}

QString NewDatasetDialog::name()
{
    return m_ui->leName->text();
}

QString NewDatasetDialog::type()
{
    return m_ui->cbType->currentText();
}

void NewDatasetDialog::setName(QString str)
{
    return m_ui->leName->setText(str);
}
