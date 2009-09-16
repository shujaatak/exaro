#ifndef MEMOHELPER_H
#define MEMOHELPER_H

#include <QtGui/QDialog>
#include <itemhelper.h>

namespace Ui {
    class MemoHelper;
}

class Memo;

class MemoHelper : public Report::ItemHelper
{
    Q_OBJECT
public:
    MemoHelper(QWidget *parent = 0);
    ~MemoHelper();

    virtual Report::ItemHelper * createInstance();
    virtual void setItem(Report::ItemInterface * item);

protected:
    void changeEvent(QEvent *e);

protected slots:
    void accept();

private:
    Ui::MemoHelper *m_ui;
    Memo * m_item;
};

#endif // MEMOHELPER_H
