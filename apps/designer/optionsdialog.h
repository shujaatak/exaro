#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include "ui_optionsdialog.h"

class OptionsDialog : public QDialog, private Ui::OptionsDialog {
    Q_OBJECT
    Q_DISABLE_COPY(OptionsDialog)
public:
    explicit OptionsDialog(QWidget *parent = 0);

private slots:
    void on_bDesignerPath_clicked();
    void storeAll();
    void restoreAll();
    void needAccept();

};

#endif // OPTIONSDIALOG_H
