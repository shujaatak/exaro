#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class ProcessDialog;
}

class ProcessDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ProcessDialog)
public:
    explicit ProcessDialog(QWidget *parent = 0);
    virtual ~ProcessDialog();

public slots:
    void showProcess(QString str);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ProcessDialog *m_ui;
};

#endif // PROCESSDIALOG_H
