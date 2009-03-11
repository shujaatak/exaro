#include "processdialog.h"
#include "ui_processdialog.h"
#include <QMovie>

ProcessDialog::ProcessDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ProcessDialog)
{
    m_ui->setupUi(this);
    QMovie *movie = new QMovie(this);
    movie->setFileName(":images/wait_animation.gif");
    m_ui->animation->setMovie(movie);
    movie->start();

//    m_ui->bCancel->setVisible(false);
}

ProcessDialog::~ProcessDialog()
{
    delete m_ui;
}

void ProcessDialog::showProcess(QString str)
{
    qDebug("ProcessDialog::showProcess(%s)", qPrintable(str));
    m_ui->processMessage->setText(str);
}

void ProcessDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
