#include "memohelper.h"
#include "memo.h"
#include "ui_memohelper.h"

MemoHelper::MemoHelper(QWidget *parent) :
    ItemHelper(parent),
    m_ui(new Ui::MemoHelper)
{
    m_ui->setupUi(this);
    this->setWindowTitle( tr("Memo Helper") );
}

MemoHelper::~MemoHelper()
{
    delete m_ui;
}

void MemoHelper::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

Report::ItemHelper * MemoHelper::createInstance()
{
    return new MemoHelper();
}

void MemoHelper::setItem(Report::ItemInterface * item)
{
    m_item = dynamic_cast<Memo*> (item);
    if (m_item)
    {
	m_ui->text->setPlainText(m_item->text());
    }
}

void MemoHelper::accept()
{
    Memo * item = dynamic_cast<Memo*> (m_item);
    if (item)
    {
	item->setText(m_ui->text->toPlainText());
    }

    Report::ItemHelper::accept();
}
