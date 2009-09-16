#ifndef ITEMHELPER_H
#define ITEMHELPER_H

#include <QDialog>

namespace Report
{
class ItemInterface;

class ItemHelper : public QDialog
{
public:
    ItemHelper( QWidget * parent = 0, Qt::WindowFlags f = 0 );

    virtual ItemHelper * createInstance() = 0;
    virtual void setItem(ItemInterface * item) = 0;

protected:
//    ItemInterface * m_item;
};
}

#endif // ITEMHELPER_H
