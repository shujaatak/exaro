#include "optionsdialog.h"
#include <QtGui>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    connect(buttonBox, SIGNAL(accepted()),SLOT(needAccept() ) );

    QTimer::singleShot(0,this,SLOT(restoreAll()));
}

void OptionsDialog::on_bDesignerPath_clicked()
{
#if defined(Q_OS_WIN)
        QString fileName=QFileDialog::getOpenFileName(this,"designer.exe",".","designer.exe");
#else
        QString fileName=QFileDialog::getOpenFileName(this,"designer","/usr/bin","designer");
#endif

    leDesignerPath->setText(fileName);

}

void OptionsDialog::needAccept()
{
    storeAll();
    emit accept();
}

void OptionsDialog::storeAll()
{
    QSettings s;
    s.setValue("Options/designerPath",leDesignerPath->text());
    s.setValue("Options/iconSize", cbIconSize->currentText ());
}

void OptionsDialog::restoreAll()
{
    QSettings s;
    leDesignerPath->setText(s.value("Options/designerPath").toString());
    cbIconSize->setCurrentIndex (cbIconSize->findText (s.value("Options/iconSize").toString() ) );
}

