#ifndef NEWDATASETDIALOG_H
#define NEWDATASETDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class NewDatasetDialog;
}

class NewDatasetDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(NewDatasetDialog)
public:
    explicit NewDatasetDialog(QWidget *parent = 0);
    virtual ~NewDatasetDialog();
    void addTypes(QStringList str);
    void addType(QString str);
    void setName(QString str);
    QString name();
    QString type();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::NewDatasetDialog *m_ui;
};

#endif // NEWDATASETDIALOG_H
